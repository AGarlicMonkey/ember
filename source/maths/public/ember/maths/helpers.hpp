#pragma once

#include "ember/maths/types.hpp"

namespace ember::inline maths {
    template<typename T>
    constexpr bool are_floats_equal(T a, T b) requires std::is_floating_point_v<T>;

    template<typename T>
    constexpr T as_radians(T degrees);

    template<typename T>
    constexpr T as_degrees(T radians);

    template<typename T>
    mat<4, 4, T> create_perspective_matrix(T fov_y, T aspect, T z_near, T z_far);

    template<typename T>
    mat<4, 4, T> look_at(vec<3, T> const &eye, vec<3, T> const &center, vec<3, T> const &up);
}

#include "helpers.inl"