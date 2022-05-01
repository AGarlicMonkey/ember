#pragma once

#include "ember/platform/input.hpp"

#include <cinttypes>

namespace ember::platform {
    mouse_button convert_button(std::uint8_t button_code);
    key convert_key(std::uint8_t key_code);
}