#include "ember/maths/helpers.hpp"

#include <cinttypes>

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
    mat<R, C, T>::column_val::column_val() = default;

    template<size_t R, size_t C, number T>
    mat<R, C, T>::column_val::column_val(std::initializer_list<T> const list) {
        col.insert(col.begin(), list.begin(), list.end());
    }

    template<size_t R, size_t C, number T>
    constexpr T &mat<R, C, T>::column_val::operator[](size_t const index) {
        return col[index];
    }

    template<size_t R, size_t C, number T>
    constexpr T const &mat<R, C, T>::column_val::operator[](size_t const index) const {
        return col[index];
    }

    template<size_t R, size_t C, number T>
    mat<R, C, T>::column_ref::column_ref() = default;

    template<size_t R, size_t C, number T>
    mat<R, C, T>::column_ref::column_ref(std::initializer_list<std::reference_wrapper<T>> const list) {
        col.insert(col.begin(), list.begin(), list.end());
    }

    template<size_t R, size_t C, number T>
    typename mat<R, C, T>::column_ref &mat<R, C, T>::column_ref::operator=(column_ref const &other) {
        for(size_t i{ 0 }; i < C; ++i) {
            col[i].get() = other[i];
        }

        return *this;
    }

    template<size_t R, size_t C, number T>
    typename mat<R, C, T>::column_ref &mat<R, C, T>::column_ref::operator=(column_val const &other) {
        for(size_t i{ 0 }; i < C; ++i) {
            col[i].get() = other[i];
        }

        return *this;
    }

    template<size_t R, size_t C, number T>
    constexpr T &mat<R, C, T>::column_ref::operator[](size_t const index) {
        return col[index];
    }

    template<size_t R, size_t C, number T>
    constexpr T const &mat<R, C, T>::column_ref::operator[](size_t const index) const {
        return col[index];
    }

    template<size_t R, size_t C, number T>
    constexpr mat<R, C, T>::mat(T val) {
        for(std::size_t r{ 0 }; r < R; ++r) {
            for(std::size_t c{ 0 }; c < C; ++c) {
                data[r + (c * C)] = r == c ? val : T{};
            }
        }
    }

    template<size_t R, size_t C, number T>
    constexpr vec<C, T> operator*(mat<R, C, T> const &m, vec<C, T> const &v) {
        vec<C, T> result{};

        for(std::size_t r{ 0 }; r < R; ++r) {
            for(std::size_t c{ 0 }; c < C; ++c) {
                result[r] += m[r][c] * v[c];
            }
        }

        return result;
    }

    template<size_t R1, size_t R2, size_t C, number T>
    constexpr mat<R1, R2, T> operator*(mat<R1, C, T> const &a, mat<C, R2, T> const &b) {
        mat<R1, R2, T> result{};

        for(std::size_t r1{ 0 }; r1 < R1; ++r1) {
            for(std::size_t r2{ 0 }; r2 < R2; ++r2) {
                for(std::size_t c{ 0 }; c < C; ++c) {
                    result[r1][r2] += a[r1][c] * b[c][r2];
                }
            }
        }

        return result;
    }

    template<size_t R, size_t C, number T>
    constexpr bool operator==(mat<R, C, T> const &lhs, mat<R, C, T> const &rhs) {
        for(std::size_t r{ 0 }; r < R; ++r) {
            for(std::size_t c{ 0 }; c < C; ++c) {
                if constexpr(std::is_floating_point_v<T>) {
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

    template<size_t R, size_t C, number T>
    constexpr bool operator!=(mat<R, C, T> const &lhs, mat<R, C, T> const &rhs) {
        return !(lhs == rhs);
    }

    template<size_t R, size_t C, number T>
    constexpr typename mat<R, C, T>::column_ref mat<R, C, T>::operator[](size_t const index) {
        column_ref column{};

        for(std::size_t c{ 0 }; c < C; ++c) {
            column.col.push_back(data[index + (c * C)]);
        }

        return column;
    }

    template<size_t R, size_t C, number T>
    constexpr typename mat<R, C, T>::column_val const mat<R, C, T>::operator[](size_t const index) const {
        column_val column{};

        for(std::size_t c{ 0 }; c < C; ++c) {
            column.col.push_back(data[index + (c * C)]);
        }

        return column;
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
    constexpr mat<4, 4, T> translate(mat<4, 4, T> const &m, vec<3, T> const &v) {
        mat<4, 4, T> result{ m };

        for(std::size_t i{ 0 }; i < 3; ++i) {
            result[i][3] = (m[i][0] * v[0]) + (m[i][1] * v[1]) + (m[i][2] * v[2]) + m[i][3];
        }

        return result;
    }

    template<number T>
    constexpr mat<4, 4, T> rotate(mat<4, 4, T> const &m, float angle, vec<3, T> axis) {
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

        mat<4, 4, T> const accum{ m * (res_x * res_y * res_z) };

        return accum;
    }

    template<number T>
    constexpr mat<4, 4, T> scale(mat<4, 4, T> const &m, vec<3, T> const &v) {
        mat<4, 4, T> result{};

        result[0] = m[0] * v[0];
        result[1] = m[1] * v[1];
        result[2] = m[2] * v[2];
        result[3] = m[3];

        return result;
    }
}