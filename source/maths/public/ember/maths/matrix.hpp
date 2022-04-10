#pragma once

#include "concepts.hpp"
#include "vector.hpp"

#include <array>
#include <cstddef>
#include <vector>

//Matrix types
namespace ember::maths {
    template<size_t R, size_t C, number T>
    struct mat {
        //TYPES
    public:
        struct column_val;
        struct column_ref;

        struct column_val {
            std::vector<T> col;

            column_val();
            column_val(std::initializer_list<T> const list);

            friend constexpr column_val operator*(column_val const &c, T scalar) {
                column_val result{};
                result.col.resize(C);

                for(size_t i{ 0 }; i < C; ++i) {
                    result[i] = c[i] * scalar;
                }

                return result;
            }

            friend constexpr bool operator==(column_val const &lhs, column_val const &rhs) {
                bool result{ true };

                for(size_t i{ 0 }; i < C && result; ++i) {
                    result = lhs[i] == rhs[i];
                }

                return result;
            }
            friend constexpr bool operator==(column_val const &lhs, column_ref const &rhs) {
                bool result{ true };

                for(size_t i{ 0 }; i < C && result; ++i) {
                    result = lhs[i] == rhs[i];
                }

                return result;
            }

            constexpr T &operator[](size_t const index);
            constexpr T const &operator[](size_t const index) const;
        };

        struct column_ref {
            std::vector<std::reference_wrapper<T>> col;

            column_ref();
            column_ref(std::initializer_list<std::reference_wrapper<T>> const list);

            column_ref &operator=(column_ref const &other);
            column_ref &operator=(column_val const &other);

            friend constexpr column_val operator*(column_ref const &c, T scalar) {
                column_val result{};
                result.col.resize(C);

                for(size_t i{ 0 }; i < C; ++i) {
                    result[i] = c[i] * scalar;
                }

                return result;
            }

            friend constexpr bool operator==(column_ref const &lhs, column_ref const &rhs) {
                bool result{ true };

                for(size_t i{ 0 }; i < C && result; ++i) {
                    result = lhs[i] == rhs[i];
                }

                return result;
            }
            friend constexpr bool operator==(column_ref const &lhs, column_val const &rhs) {
                bool result{ true };

                for(size_t i{ 0 }; i < C && result; ++i) {
                    result = lhs[i] == rhs[i];
                }

                return result;
            }

            constexpr T &operator[](size_t const index);
            constexpr T const &operator[](size_t const index) const;
        };

        //VARIABLES
    public:
        std::array<T, R * C> data{};

        //FUNCTIONS
    public:
        constexpr mat() = default;
        constexpr mat(T val);

        template<size_t R1, size_t C1, number U>
        friend constexpr vec<C1, U> operator*(mat<R1, C1, U> const &m, vec<C1, U> const &v);

        template<size_t R1, size_t R2, size_t C1, number U>
        friend constexpr mat<R1, R2, U> operator*(mat<R1, C1, U> const &a, mat<C1, R2, U> const &b);

        template<size_t R1, size_t C1, number U>
        friend constexpr bool operator==(mat<R1, C1, U> const &lhs, mat<R1, C1, U> const &rhs);
        template<size_t R1, size_t C1, number U>
        friend constexpr bool operator!=(mat<R1, C1, U> const &lhs, mat<R1, C1, U> const &rhs);

        constexpr column_ref operator[](size_t const index);
        constexpr column_val const operator[](size_t const index) const;
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
    constexpr mat<4, 4, T> translate(mat<4, 4, T> const &m, vec<3, T> const &v);

    /**
     * @brief Rotates matrix m with angle along v.
     * @tparam T The type of the matrix and vector.
     * @param m 
     * @param angle The angle in radians to rotate by.
     * @param axis Multiplied by the angle to know how much to rotate around each axis.
     * @return 
     */
    template<number T>
    constexpr mat<4, 4, T> rotate(mat<4, 4, T> const &m, float angle, vec<3, T> axis);

    /**
     * @brief Scales matrix m by vector v.
     * @tparam T The type of the matrix and vector.
     * @param m 
     * @param v 
     * @return 
     */
    template<number T>
    constexpr mat<4, 4, T> scale(mat<4, 4, T> const &m, vec<3, T> const &v);
}

#include "matrix.inl"