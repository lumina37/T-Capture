#include <dbus/dbus.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class ScreenCapturePortal {
private:
    DBusConnection* connection;
    std::string session_handle;

public:
    ScreenCapturePortal() : connection(nullptr) {}

    ~ScreenCapturePortal() {
        if (connection) {
            dbus_connection_unref(connection);
        }
    }

    bool initialize() {
        DBusError error;
        dbus_error_init(&error);

        // 连接到会话总线
        connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
        if (dbus_error_is_set(&error)) {
            std::cerr << "连接D-Bus失败: " << error.message << std::endl;
            dbus_error_free(&error);
            return false;
        }

        if (!connection) {
            std::cerr << "无法获取D-Bus连接" << std::endl;
            return false;
        }

        return true;
    }

    std::string generateHandle() {
        pid_t pid = getpid();
        static int counter = 0;
        return "/org/freedesktop/portal/desktop/request/" + std::to_string(pid) + "_" + std::to_string(counter++);
    }

    bool createSession() {
        DBusMessage* message =
            dbus_message_new_method_call("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
                                         "org.freedesktop.portal.ScreenCast", "CreateSession");

        if (!message) {
            std::cerr << "创建D-Bus消息失败" << std::endl;
            return false;
        }

        // 准备参数
        std::string handle = generateHandle();
        DBusMessageIter iter, dict_iter, variant_iter;

        dbus_message_iter_init_append(message, &iter);

        // 添加选项字典
        dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &dict_iter);

        // 添加 handle_token
        const char* handle_token_key = "session_handle_token";
        const char* handle_token_value = "unique_token_123";

        DBusMessageIter entry_iter;
        dbus_message_iter_open_container(&dict_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
        dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &handle_token_key);
        dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "s", &variant_iter);
        dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_STRING, &handle_token_value);
        dbus_message_iter_close_container(&entry_iter, &variant_iter);
        dbus_message_iter_close_container(&dict_iter, &entry_iter);

        dbus_message_iter_close_container(&iter, &dict_iter);

        // 发送消息
        DBusError err;
        dbus_error_init(&err);
        DBusMessage* reply =
            dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, &err);

        dbus_message_unref(message);

        if (dbus_error_is_set(&err)) {
            std::cerr << "D-Bus Error: " << err.message << std::endl;
            dbus_error_free(&err);
            return false;
        }

        // 解析响应
        DBusMessageIter reply_iter;
        dbus_message_iter_init(reply, &reply_iter);

        const char* object_path;
        dbus_message_iter_get_basic(&reply_iter, &object_path);
        session_handle = object_path;

        dbus_message_unref(reply);

        std::cout << "创建会话成功: " << session_handle << std::endl;
        return true;
    }

    bool selectSources() {
        DBusMessage* message =
            dbus_message_new_method_call("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
                                         "org.freedesktop.portal.ScreenCast", "SelectSources");

        if (!message) {
            std::cerr << "创建SelectSources消息失败" << std::endl;
            return false;
        }

        DBusMessageIter iter, dict_iter, variant_iter;
        dbus_message_iter_init_append(message, &iter);

        // 添加会话句柄
        const char* session_handle_cstr = session_handle.c_str();
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_OBJECT_PATH, &session_handle_cstr);

        // 添加选项字典
        dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &dict_iter);

        // 添加源类型 (monitor 和 window)
        const char* types_key = "types";
        uint32_t types_value = 1;  // MONITOR | WINDOW

        DBusMessageIter entry_iter;
        dbus_message_iter_open_container(&dict_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
        dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &types_key);
        dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "u", &variant_iter);
        dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_UINT32, &types_value);
        dbus_message_iter_close_container(&entry_iter, &variant_iter);
        dbus_message_iter_close_container(&dict_iter, &entry_iter);

        // 添加多选项
        const char* multiple_key = "multiple";
        dbus_bool_t multiple_value = TRUE;

        dbus_message_iter_open_container(&dict_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry_iter);
        dbus_message_iter_append_basic(&entry_iter, DBUS_TYPE_STRING, &multiple_key);
        dbus_message_iter_open_container(&entry_iter, DBUS_TYPE_VARIANT, "b", &variant_iter);
        dbus_message_iter_append_basic(&variant_iter, DBUS_TYPE_BOOLEAN, &multiple_value);
        dbus_message_iter_close_container(&entry_iter, &variant_iter);
        dbus_message_iter_close_container(&dict_iter, &entry_iter);

        dbus_message_iter_close_container(&iter, &dict_iter);

        // 发送消息
        DBusMessage* reply =
            dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, nullptr);

        dbus_message_unref(message);

        if (!reply) {
            std::cerr << "选择源失败" << std::endl;
            return false;
        }

        dbus_message_unref(reply);
        std::cout << "选择源成功" << std::endl;
        return true;
    }

    bool startCapture() {
        DBusMessage* message =
            dbus_message_new_method_call("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop",
                                         "org.freedesktop.portal.ScreenCast", "Start");

        if (!message) {
            std::cerr << "创建Start消息失败" << std::endl;
            return false;
        }

        DBusMessageIter iter, dict_iter;
        dbus_message_iter_init_append(message, &iter);

        // 添加会话句柄
        const char* session_handle_cstr = session_handle.c_str();
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_OBJECT_PATH, &session_handle_cstr);

        // 添加父窗口 (空字符串表示无父窗口)
        const char* parent_window = "";
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &parent_window);

        // 添加空选项字典
        dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &dict_iter);
        dbus_message_iter_close_container(&iter, &dict_iter);

        // 发送消息
        DBusMessage* reply =
            dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, nullptr);

        dbus_message_unref(message);

        if (!reply) {
            std::cerr << "开始捕获失败" << std::endl;
            return false;
        }

        // 解析响应获取流信息
        DBusMessageIter reply_iter, variant_iter, array_iter;
        dbus_message_iter_init(reply, &reply_iter);

        // 跳过响应码
        dbus_message_iter_next(&reply_iter);

        // 获取结果字典
        dbus_message_iter_recurse(&reply_iter, &variant_iter);

        std::cout << "开始捕获成功" << std::endl;

        // 这里可以进一步解析流信息，包括PipeWire流ID等

        dbus_message_unref(reply);
        return true;
    }

    bool requestScreenCapture() {
        if (!initialize()) {
            return false;
        }

        if (!createSession()) {
            return false;
        }

        if (!selectSources()) {
            return false;
        }

        // if (!startCapture()) {
        //     return false;
        // }

        return true;
    }
};

int main() {
    ScreenCapturePortal portal;

    if (portal.requestScreenCapture()) {
        std::cout << "屏幕捕获权限申请成功！" << std::endl;
        std::cout << "现在可以通过PipeWire进行屏幕捕获了" << std::endl;
    } else {
        std::cout << "屏幕捕获权限申请失败！" << std::endl;
        return 1;
    }

    return 0;
}