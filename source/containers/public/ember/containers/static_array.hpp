#pragma once

#include <array>

namespace ember::containers {
    template<typename type, std::size_t size>
    using static_array = std::array<type, size>;
}