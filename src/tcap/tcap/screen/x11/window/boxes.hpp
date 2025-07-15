#pragma once

#include <expected>
#include <vector>

#include "tcap/screen/x11/display/box.hpp"
#include "tcap/common/defines.h"
#include "tcap/screen/x11/screen.hpp"
#include "tcap/screen/x11/window/box.hpp"
#include "tcap/utils/error.hpp"

namespace tcap::x11 {

class TopWindowBoxes {
    TopWindowBoxes(std::vector<WindowBox>&& windowBoxes) noexcept;

public:
    TopWindowBoxes& operator=(const TopWindowBoxes&) = delete;
    TopWindowBoxes(const TopWindowBoxes&) = delete;
    TCAP_API TopWindowBoxes(TopWindowBoxes&& rhs) noexcept = default;
    TCAP_API TopWindowBoxes& operator=(TopWindowBoxes&& rhs) noexcept = default;
    TCAP_API ~TopWindowBoxes() noexcept = default;

    [[nodiscard]] TCAP_API static std::expected<TopWindowBoxes, Error> create(const DisplayBox& displayBox,
                                                                              const ScreenBox& screenBox) noexcept;

    [[nodiscard]] TCAP_API const std::vector<WindowBox>& getWindowBoxes() const noexcept { return windowBoxes_; }
    [[nodiscard]] TCAP_API std::vector<WindowBox>& getWindowBoxes() noexcept { return windowBoxes_; }
    [[nodiscard]] TCAP_API const WindowBox& getWindowBox(const int index) const noexcept { return windowBoxes_[index]; }
    [[nodiscard]] TCAP_API WindowBox& getWindowBox(const int index) noexcept { return windowBoxes_[index]; }

private:
    std::vector<WindowBox> windowBoxes_;
};

}  // namespace tcap::x11
