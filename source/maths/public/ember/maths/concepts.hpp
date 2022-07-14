#pragma once

#include <concepts>

namespace ember::inline maths {
    template<typename T>
    concept number = std::integral<T> || std::floating_point<T>;
}