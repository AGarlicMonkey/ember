#pragma once

#include "ember/maths/types.hpp"

namespace ember::maths {
    template<typename T>
    constexpr bool are_floats_equal(T a, T b) requires std::is_floating_point_v<T>;

    template<typename T>
    constexpr T as_radians(T degrees);

    template<typename T>
    constexpr T as_degrees(T radians);

}

#include "helpers.inl"