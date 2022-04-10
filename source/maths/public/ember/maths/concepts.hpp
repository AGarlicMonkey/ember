#pragma once

#include <concepts>

namespace ember::maths {
    template<typename T>
    concept number = std::integral<T> || std::floating_point<T>;
}