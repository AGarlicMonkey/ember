#include "ember/maths/helpers.hpp"

#include <cinttypes>
#include <ember/core/log.hpp>

namespace ember::maths {
    namespace internal {
        template<size_t N, std::floating_point T>
        mat<N, N, T> get_cofactor(mat<N, N, T> const &m, int32_t p, int32_t q, int32_t n) {
            std::int32_t i{ 0 };
            std::int32_t j{ 0 };
            mat<N, N, T> result{};

            for(std::int32_t r{ 0 }; r < n; ++r) {
                for(std::int32_t c{ 0 }; c < n; ++c) {
                    if(r != p && c != q) {
                        result[i][j++] = m[r][c];
                        if(j == n - 1) {
                            j = 0;
                            i++;
                        }
                    }
                }
            }

            return result;
        }

        template<size_t N, std::floating_point T>
        T get_determinant(mat<N, N, T> const &m, int32_t n) {
            T determinant{ 0 };

            if(n != 1) {
                std::int32_t sign{ 1 };
                for(std::int32_t f{ 0 }; f < n; ++f) {
                    mat<N, N, T> const cofactors{ get_cofactor(m, 0, f, n) };
                    determinant += sign * m[0][f] * get_determinant(cofactors, n - 1);
                    sign = -sign;
                }
            } else {
                determinant = m[0][0];
            }

            return determinant;
        }

        template<size_t N, std::floating_point T>
        mat<N, N, T> get_adjoint(mat<N, N, T> const &m) {
            mat<N, N, T> adj{};

            if(N != 1) {
                std::int32_t sign{ 1 };
                for(std::int32_t i{ 0 }; i < N; ++i) {
                    for(std::int32_t j{ 0 }; j < N; ++j) {
                        mat<N, N, T> const cofactors{ get_cofactor(m, i, j, N) };
                        sign      = ((i + j) % 2 == 0) ? 1 : -1;
                        adj[j][i] = sign * (get_determinant(cofactors, N - 1));//Interchange rows and columns to get the transpose of the cofactor matrix
                    }
                }
            } else {
                adj[0][0] = 1;
            }

            return adj;
        }
    }

    template<size_t R, size_t C, number T>
    constexpr mat<R, C, T>::mat(T val) {
        for(std::size_t r{ 0 }; r < R; ++r) {
            for(std::size_t c{ 0 }; c < C; ++c) {
                data[r][c] = r == c ? val : T{};
            }
        }
    }

    template<size_t R1, size_t C1, number U>
    constexpr typename mat<R1, C1, U>::col_type operator*(mat<R1, C1, U> const &m, typename mat<R1, C1, U>::row_type const &v) {
        typename mat<R1, C1, U>::col_type result{};

        for(std::size_t r{ 0 }; r < R1; ++r) {
            for(std::size_t c{ 0 }; c < C1; ++c) {
                result[r] += m[c][r] * v[c];
            }
        }

        return result;
    }

    template<size_t R1, size_t C1, number U>
    constexpr mat<C1, C1, U> operator*(mat<R1, C1, U> const &a, mat<C1, R1, U> const &b) {
        mat<C1, C1, U> result{};

        for(std::size_t c1{ 0 }; c1 < C1; ++c1) {
            for(std::size_t c2{ 0 }; c2 < C1; ++c2) {
                for(std::size_t r1{ 0 }; r1 < R1; ++r1) {
                    result[c1][c2] += a[r1][c2] * b[c1][r1];
                }
            }
        }

        return result;
    }

    template<size_t R1, size_t C1, number U>
    constexpr bool operator==(mat<R1, C1, U> const &lhs, mat<R1, C1, U> const &rhs) {
        for(std::size_t r{ 0 }; r < R1; ++r) {
            for(std::size_t c{ 0 }; c < C1; ++c) {
                if constexpr(std::is_floating_point_v<U>) {
                    if(!are_floats_equal(lhs[r][c], rhs[r][c])) {
                        return false;
                    }
                } else {
                    if(lhs[r][c] != rhs[r][c]) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    template<size_t R1, size_t C1, number U>
    constexpr bool operator!=(mat<R1, C1, U> const &lhs, mat<R1, C1, U> const &rhs) {
        return !(lhs == rhs);
    }

    template<size_t R, size_t C, number T>
    constexpr typename mat<R, C, T>::col_type &mat<R, C, T>::operator[](size_t const index) {
        return data[index];
    }

    template<size_t R, size_t C, number T>
    constexpr typename mat<R, C, T>::col_type const &mat<R, C, T>::operator[](size_t const index) const {
        return data[index];
    }

    template<size_t R, size_t C, number T>
    constexpr mat<R, C, T> transpose(mat<R, C, T> const &m) {
        mat<R, C, T> transposed{};

        for(std::size_t r{ 0 }; r < R; ++r) {
            for(std::size_t c{ 0 }; c < C; ++c) {
                transposed[r][c] = m[c][r];
            }
        }

        return transposed;
    }

    template<size_t N, std::floating_point T>
    constexpr mat<N, N, T> inverse(mat<N, N, T> const &m) {
        T const determinant{ internal::get_determinant(m, N) };
        EMBER_CHECK_MSG(determinant != 0.0f, "Can't inverse matrix. Determinant cannot be 0");
        mat<N, N, T> const adj{ internal::get_adjoint(m) };

        mat<N, N, T> result;
        for(std::size_t i{ 0 }; i < N; ++i) {
            for(std::size_t j{ 0 }; j < N; ++j) {
                result[i][j] = adj[i][j] / determinant;
            }
        }

        return result;
    }

    template<number T>
    constexpr mat<4, 4, T> translate(vec<3, T> const &v) {
        mat<4, 4, T> result{ 1.0f };

        result[3] = { v.x, v.y, v.z, 1.0f };

        return result;
    }

    template<number T>
    constexpr mat<4, 4, T> rotate(float angle, vec<3, T> axis) {
        float const c{ std::cos(angle) };
        float const s{ std::sin(angle) };

        axis = normalise(axis);

        mat<4, 4, T> res_x{ 1.0f };
        res_x[1][1] = axis.x != 0 ? c * axis.x : 1;
        res_x[1][2] = -s * axis.x;
        res_x[2][1] = s * axis.x;
        res_x[2][2] = axis.x != 0 ? c * axis.x : 1;

        mat<4, 4, T> res_y{ 1.0f };
        res_y[0][0] = axis.y != 0 ? c * axis.y : 1;
        res_y[0][2] = s * axis.y;
        res_y[2][0] = -s * axis.y;
        res_y[2][2] = axis.y != 0 ? c * axis.y : 1;

        mat<4, 4, T> res_z{ 1.0f };
        res_z[0][0] = axis.z != 0 ? c * axis.z : 1;
        res_z[0][1] = -s * axis.z;
        res_z[1][0] = s * axis.z;
        res_z[1][1] = axis.z != 0 ? c * axis.z : 1;

        mat<4, 4, T> const accum{ res_x * res_y * res_z };

        return accum;
    }

    template<number T>
    constexpr mat<4, 4, T> rotate(vec<3, T> const &angles) {
        float const c1{ std::cos(-angles.x) };
        float const c2{ std::cos(-angles.y) };
        float const c3{ std::cos(-angles.z) };
        
        float const s1{ std::sin(-angles.x) };
        float const s2{ std::sin(-angles.y) };
        float const s3{ std::sin(-angles.z) };

        mat<4, 4, T> result{};

        result[0][0] = c2 * c3;
        result[0][1] = -c1 * s3 + s1 * s2 * c3;
        result[0][2] = s1 * s3 + c1 * s2 * c3;
        result[0][3] = 0;
        result[1][0] = c2 * s3;
        result[1][1] = c1 * c3 + s1 * s2 * s3;
        result[1][2] = -s1 * c3 + c1 * s2 * s3;
        result[1][3] = 0;
        result[2][0] = -s2;
        result[2][1] = s1 * c2;
        result[2][2] = c1 * c2;
        result[2][3] = 0;
        result[3][0] = 0;
        result[3][1] = 0;
        result[3][2] = 0;
        result[3][3] = 1;

        return result;
    }

    template<number T>
    constexpr mat<4, 4, T> scale(vec<3, T> const &v) {
        mat<4, 4, T> result{};

        result[0] = { v.x, 0, 0, 0 };
        result[1] = { 0, v.y, 0, 0 };
        result[2] = { 0, 0, v.z, 0 };
        result[3] = { 0, 0, 0, 1 };

        return result;
    }
}