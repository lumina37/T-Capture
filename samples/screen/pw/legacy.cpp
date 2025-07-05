#include <fstream>
#include <print>
#include <vector>

#include "../../sample_helper.hpp"
#include "tcap.hpp"

int main() {
    tcap::dbus::ConnectionBox connBox = tcap::dbus::ConnectionBox::create() | unwrap;
}
