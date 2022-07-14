#pragma once

#include "concepts.hpp"
#include "vector.hpp"

#include <cstddef>
#include <ember/containers/static_array.hpp>
#include <vector>

//Matrix types
namespace ember::inline maths {
    template<size_t R, size_t C, number T>
    struct mat {
        //TYPES
    public:
        using row_type = vec<R, T>;
        using col_type = vec<C, T>;

        //VARIABLES
    public:
        static_array<col_type, R> data{};

        //FUNCTIONS
    public:
        constexpr mat() = default;
        constexpr mat(T val);

        template<size_t R1, size_t C1, number U>
        friend constexpr typename mat<R1, C1, U>::col_type operator*(mat<R1, C1, U> const &m, typename mat<R1, C1, U>::row_type const &v);

        template<size_t R1, size_t C1, number U>
        friend constexpr mat<C1, C1, U> operator*(mat<R1, C1, U> const &a, mat<C1, R1, U> const &b);

        template<size_t R1, size_t C1, number U>
        friend constexpr bool operator==(mat<R1, C1, U> const &lhs, mat<R1, C1, U> const &rhs);
        template<size_t R1, size_t C1, number U>
        friend constexpr bool operator!=(mat<R1, C1, U> const &lhs, mat<R1, C1, U> const &rhs);

        constexpr col_type &operator[](size_t const index);
        constexpr col_type const &operator[](size_t const index) const;
    };
}

//Matrix aliases
namespace ember::maths {
    using mat3f = mat<3, 3, float>;
    using mat4f = mat<4, 4, float>;

    using mat3d = mat<3, 3, double>;
    using mat4d = mat<4, 4, double>;
}

//Matrix operations
namespace ember::maths {
    /**
     * @brief Returns the transpose of m.
     * @tparam R How many rows the matrix has.
     * @tparam C How many columns the matrix has.
     * @tparam T The type of the matrix.
     * @param m 
     * @return 
     */
    template<size_t R, size_t C, number T>
    constexpr mat<R, C, T> transpose(mat<R, C, T> const &m);

    /**
     * @brief Returns the inverse of m.
     * @tparam N How many rows & columns the matrix has
     * @tparam T The type of the matrix.
     * @param m 
     * @return 
     */
    template<size_t N, std::floating_point T>
    constexpr mat<N, N, T> inverse(mat<N, N, T> const &m);

    /**
     * @brief Translates matrix m with vector v.
     * @tparam T The type of the matrix and vector.
     * @param m 
     * @param v 
     * @return 
     */
    template<number T>
    constexpr mat<4, 4, T> translate(vec<3, T> const &v);

    /**
     * @brief Rotates matrix m with by angle along axis.
     * @tparam T The type of the matrix and vector.
     * @param m 
     * @param angle The angle in radians to rotate by.
     * @param axis Multiplied by the angle to know how much to rotate around each axis.
     * @return 
     */
    template<number T>
    constexpr mat<4, 4, T> rotate(float angle, vec<3, T> axis);

    /**
     * @brief Converts euler angles into a rotation matrix
     * @tparam T 
     * @param m 
     * @param angles Angle of each component in radians.
     * @return 
     */
    template<number T>
    constexpr mat<4, 4, T> rotate(vec<3, T> const &angles);

    /**
     * @brief Scales matrix m by vector v.
     * @tparam T The type of the matrix and vector.
     * @param m 
     * @param v 
     * @return 
     */
    template<number T>
    constexpr mat<4, 4, T> scale(vec<3, T> const &v);
}

#include "matrix.inl"