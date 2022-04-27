
namespace ember::maths {
    namespace internal {
        template<std::size_t size>
        struct size_to_int_type;

        template<>
        struct size_to_int_type<4> {
            using int_t  = int32_t;
            using uint_t = uint32_t;
        };

        template<>
        struct size_to_int_type<8> {
            using int_t  = int64_t;
            using uint_t = uint64_t;
        };
    }

    template<typename T>
    constexpr bool are_floats_equal(T a, T b) requires std::is_floating_point_v<T> {
        using int_t = typename internal::size_to_int_type<sizeof(T)>::int_t;

        union fp_union {
            T val;
            int_t bits;
        };

        std::size_t constexpr num_bits{ 8 * sizeof(T) };
        int_t constexpr sign_mask{ static_cast<int_t>(1) << (num_bits - 1) };

        int_t constexpr max_ulps{ 4 };

        fp_union const a_union{ a };
        fp_union const b_union{ b };

        int_t const a_int{ sign_mask & a_union.bits ? ~a_union.bits + 1 : sign_mask | a_union.bits };
        int_t const b_int{ sign_mask & b_union.bits ? ~b_union.bits + 1 : sign_mask | b_union.bits };

        int_t const intDiff{ a_int >= b_int ? a_int - b_int : b_int - a_int };
        return intDiff <= max_ulps;
    }

    template<typename T>
    constexpr T as_radians(T degrees) {
        T constexpr deg_to_rad{ 0.01745329251994329576923690768489 };
        return degrees * deg_to_rad;
    }

    template<typename T>
    constexpr T as_degrees(T radians){
        T constexpr rad_to_deg{ 57.295779513082320876798154814105 };
        return radians * rad_to_deg;
    }

    template<typename T>
    mat<4, 4, T> create_perspective_matrix(T fov_y, T aspect, T z_near, T z_far) {
        T const tan_half_fov_y{ std::tan(fov_y / static_cast<T>(2)) };

        mat<4, 4, T> result{};
        result[0][0] = static_cast<T>(1) / (aspect * tan_half_fov_y);
        result[1][1] = -(static_cast<T>(1) / (tan_half_fov_y));
        result[2][2] = z_far / (z_far - z_near);
        result[2][3] = static_cast<T>(1);
        result[3][2] = -(z_far * z_near) / (z_far - z_near);

        return result;
    }

    template<typename T>
    mat<4, 4, T> look_at(vec<3, T> const &eye, vec<3, T> const &center, vec<3, T> const &up) {
        vec<3, T> const f{ normalise(center - eye) };
        vec<3, T> const s{ normalise(cross(up, f)) };
        vec<3, T> const u{ cross(f, s) };

        mat<4, 4, T> result{ 1.0f };
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] = f.x;
        result[1][2] = f.y;
        result[2][2] = f.z;
        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] = -dot(f, eye);
        return result;
    }
}