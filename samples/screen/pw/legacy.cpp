#include <fstream>
#include <print>
#include <vector>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::dbus::ConnectionBox connBox = tcap::dbus::ConnectionBox::create() | unwrap;
    tcap::dbus::ReqCreateSessionBox reqBox = tcap::dbus::ReqCreateSessionBox::create("a") | unwrap;
    tcap::dbus::ResCreateSessionBox resBox = connBox.createSession(std::move(reqBox)) | unwrap;
}
