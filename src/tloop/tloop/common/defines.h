#pragma once

#ifdef _TLOOP_LIB_DYNAMIC
#    ifdef _MSC_VER
#        ifdef tcap_lib_dynamic_EXPORTS
#            define TLOOP_API __declspec(dllexport)
#        else
#            define TLOOP_API __declspec(dllimport)
#        endif
#    else
#        ifdef tcap_lib_dynamic_EXPORTS
#            define TLOOP_API __attribute__((visibility("default")))
#        else
#            define TLOOP_API
#        endif
#    endif
#else
#    define TLOOP_API
#endif

#ifdef _WIN32
#    define TLOOP_ENABLE_IOCP 1
#else
#    define TLOOP_ENABLE_IOCP 0
#endif
