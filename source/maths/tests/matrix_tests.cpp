#include <ember/maths/matrix.hpp>
#include <gtest/gtest.h>

using namespace ember;

TEST(matrix_tests, can_create_default_matrix) {
    mat4f matrix{};

    for(size_t i{ 0 }; i < 4; ++i) {
        for(size_t j{ 0 }; j < 4; ++j) {
            EXPECT_EQ(matrix[i][j], 0.0f);
        }
    }
}

TEST(matrix_tests, can_create_identity_matrix) {
    mat4f identity{ 1.0f };

    EXPECT_EQ(identity[0][0], 1.0f);
    EXPECT_NE(identity[0][1], 1.0f);
    EXPECT_NE(identity[0][2], 1.0f);
    EXPECT_NE(identity[0][3], 1.0f);

    EXPECT_NE(identity[1][0], 1.0f);
    EXPECT_EQ(identity[1][1], 1.0f);
    EXPECT_NE(identity[1][2], 1.0f);
    EXPECT_NE(identity[1][3], 1.0f);

    EXPECT_NE(identity[2][0], 1.0f);
    EXPECT_NE(identity[2][1], 1.0f);
    EXPECT_EQ(identity[2][2], 1.0f);
    EXPECT_NE(identity[2][3], 1.0f);

    EXPECT_NE(identity[3][0], 1.0f);
    EXPECT_NE(identity[3][1], 1.0f);
    EXPECT_NE(identity[3][2], 1.0f);
    EXPECT_EQ(identity[3][3], 1.0f);
}

TEST(matrix_tests, can_multiply_matrix_by_vector) {
    {
        mat4f matrix{};
        matrix[0] = { 1, 0, 0, 9 };
        matrix[1] = { 0, 2, 0, 8 };
        matrix[2] = { 0, 4, 1, 1 };
        matrix[3] = { 0, 1, 0, 3 };

        vec4f const vector{ 2, 3, 4, 5 };

        vec4f const result{ matrix * vector };

        EXPECT_EQ(result[0], 2);
        EXPECT_EQ(result[1], 27);
        EXPECT_EQ(result[2], 4);
        EXPECT_EQ(result[3], 61);
    }

    //Simulate translating a vector
    {
        mat4f matrix{};
        matrix[0] = { 1, 0, 0, 0 };
        matrix[1] = { 0, 1, 0, 0 };
        matrix[2] = { 0, 0, 1, 0 };
        matrix[3] = { 2, 1, 3, 1 };

        vec4f const vector{ 2, 3, 4, 1 };

        vec4f const result{ matrix * vector };

        EXPECT_EQ(result[0], 4);
        EXPECT_EQ(result[1], 4);
        EXPECT_EQ(result[2], 7);
        EXPECT_EQ(result[3], 1);
    }
}

TEST(matrix_tests, can_multiply_matrix_by_matrix) {
    //Test for correct row / colum sizing
    {
        mat<3, 2, float> b{};
        b[0] = { 3, 4 };
        b[1] = { 3, -2 };
        b[2] = { 4, -2 };

        mat<2, 3, float> a{};
        a[0] = { 0, 3, 5 };
        a[1] = { 5, 5, 2 };

        mat<2, 2, float> result{ b * a };

        EXPECT_EQ(result[0][0], 29);
        EXPECT_EQ(result[0][1], -16);
        EXPECT_EQ(result[1][0], 38);
        EXPECT_EQ(result[1][1], 6);
    }

    //Test for large matrices
    {
        mat4f a{};
        a[0] = { 0, 3, 5, 3 };
        a[1] = { 1, 7, 2, 2 };
        a[2] = { 5, 1, 2, 7 };
        a[3] = { 3, 2, 2, 9 };

        mat4f b{};
        b[0] = { 3, 4, 6, 5 };
        b[1] = { 3, -2, 8, 1 };
        b[2] = { 8, -6, 9, 4 };
        b[3] = { 4, -2, 1, 6 };

        mat4f result{ a * b };

        EXPECT_EQ(result[0][0], 49);
        EXPECT_EQ(result[0][1], 53);
        EXPECT_EQ(result[0][2], 45);
        EXPECT_EQ(result[0][3], 104);

        EXPECT_EQ(result[1][0], 41);
        EXPECT_EQ(result[1][1], 5);
        EXPECT_EQ(result[1][2], 29);
        EXPECT_EQ(result[1][3], 70);

        EXPECT_EQ(result[2][0], 51);
        EXPECT_EQ(result[2][1], -1);
        EXPECT_EQ(result[2][2], 54);
        EXPECT_EQ(result[2][3], 111);

        EXPECT_EQ(result[3][0], 21);
        EXPECT_EQ(result[3][1], 11);
        EXPECT_EQ(result[3][2], 30);
        EXPECT_EQ(result[3][3], 69);
    }
}

TEST(matrix_tests, can_transpose_a_matrix) {
    mat4f matrix{};

    matrix[0] = { 0, 1, 2, 3 };
    matrix[1] = { 4, 4, 4, 4 };
    matrix[2] = { 6, 7, 6, 7 };
    matrix[3] = { 1, 1, 0, 0 };

    mat4f const transposed{ transpose(matrix) };

    EXPECT_EQ(transposed[0][0], matrix[0][0]);
    EXPECT_EQ(transposed[0][1], matrix[1][0]);
    EXPECT_EQ(transposed[0][2], matrix[2][0]);
    EXPECT_EQ(transposed[0][3], matrix[3][0]);

    EXPECT_EQ(transposed[1][0], matrix[0][1]);
    EXPECT_EQ(transposed[1][1], matrix[1][1]);
    EXPECT_EQ(transposed[1][2], matrix[2][1]);
    EXPECT_EQ(transposed[1][3], matrix[3][1]);

    EXPECT_EQ(transposed[2][0], matrix[0][2]);
    EXPECT_EQ(transposed[2][1], matrix[1][2]);
    EXPECT_EQ(transposed[2][2], matrix[2][2]);
    EXPECT_EQ(transposed[2][3], matrix[3][2]);

    EXPECT_EQ(transposed[3][0], matrix[0][3]);
    EXPECT_EQ(transposed[3][1], matrix[1][3]);
    EXPECT_EQ(transposed[3][2], matrix[2][3]);
    EXPECT_EQ(transposed[3][3], matrix[3][3]);
}

TEST(matrix_tests, can_inverse_a_matrix) {
    {
        mat3f const aIdentity{ 1 };

        mat3f a{};
        a[0] = { 3, 0, 2 };
        a[1] = { 2, 0, -2 };
        a[2] = { 0, 1, 1 };

        ASSERT_NE(a, aIdentity);

        auto aInv{ inverse(a) };

        EXPECT_EQ(a * aInv, aIdentity);
        EXPECT_EQ(aInv * a, aIdentity);
    }

    {
        mat4f const bIdentity{ 1 };

        mat4f b{};
        b[0] = { 2, 0, 0, 0 };
        b[1] = { 0, 2, 0, 0 };
        b[2] = { 0, 0, 2, 0 };
        b[3] = { 0, 0, 0, 1 };

        ASSERT_NE(b, bIdentity);

        auto bInv{ inverse(b) };

        EXPECT_EQ(b * bInv, bIdentity);
        EXPECT_EQ(bInv * b, bIdentity);
    }
}

TEST(matrix_tests, can_translate_a_matrix) {
    vec3f const v{ 3, 7, 8 };

    mat4f const translation{ translate(v) };
    mat4f result;
    result[0] = { 1, 0, 0, v.x };
    result[1] = { 0, 1, 0, v.y };
    result[2] = { 0, 0, 1, v.z };
    result[3] = { 0, 0, 0, 1 };

    EXPECT_EQ(translation[0][0], 1);
    EXPECT_EQ(translation[0][1], 0);
    EXPECT_EQ(translation[0][2], 0);
    EXPECT_EQ(translation[0][3], 0);

    EXPECT_EQ(translation[1][0], 0);
    EXPECT_EQ(translation[1][1], 1);
    EXPECT_EQ(translation[1][2], 0);
    EXPECT_EQ(translation[1][3], 0);

    EXPECT_EQ(translation[2][0], 0);
    EXPECT_EQ(translation[2][1], 0);
    EXPECT_EQ(translation[2][2], 1);
    EXPECT_EQ(translation[2][3], 0);

    EXPECT_EQ(translation[3][0], v.x);
    EXPECT_EQ(translation[3][1], v.y);
    EXPECT_EQ(translation[3][2], v.z);
    EXPECT_EQ(translation[3][3], 1);
}

TEST(matrix_tests, can_rotate_a_matrix_angle_axis) {
    vec3f const x{ 1, 0, 0 };
    vec3f const y{ 0, 1, 0 };
    vec3f const z{ 0, 0, 1 };
    float const angle{ 45.0f * 0.0174533f };

    float const c{ std::cos(angle) };
    float const s{ std::sin(angle) };

    //X
    {
        mat4f const rotationX{ rotate(angle, x) };

        EXPECT_FLOAT_EQ(rotationX[0][0], 1);
        EXPECT_FLOAT_EQ(rotationX[0][1], 0);
        EXPECT_FLOAT_EQ(rotationX[0][2], 0);
        EXPECT_FLOAT_EQ(rotationX[0][3], 0);

        EXPECT_FLOAT_EQ(rotationX[1][0], 0);
        EXPECT_FLOAT_EQ(rotationX[1][1], c);
        EXPECT_FLOAT_EQ(rotationX[1][2], -s);
        EXPECT_FLOAT_EQ(rotationX[1][3], 0);

        EXPECT_FLOAT_EQ(rotationX[2][0], 0);
        EXPECT_FLOAT_EQ(rotationX[2][1], s);
        EXPECT_FLOAT_EQ(rotationX[2][2], c);
        EXPECT_FLOAT_EQ(rotationX[2][3], 0);

        EXPECT_FLOAT_EQ(rotationX[3][0], 0);
        EXPECT_FLOAT_EQ(rotationX[3][1], 0);
        EXPECT_FLOAT_EQ(rotationX[3][2], 0);
        EXPECT_FLOAT_EQ(rotationX[3][3], 1);
    }

    //Y
    {
        mat4f const rotationY{ rotate(angle, y) };

        EXPECT_FLOAT_EQ(rotationY[0][0], c);
        EXPECT_FLOAT_EQ(rotationY[0][1], 0);
        EXPECT_FLOAT_EQ(rotationY[0][2], s);
        EXPECT_FLOAT_EQ(rotationY[0][3], 0);

        EXPECT_FLOAT_EQ(rotationY[1][0], 0);
        EXPECT_FLOAT_EQ(rotationY[1][1], 1);
        EXPECT_FLOAT_EQ(rotationY[1][2], 0);
        EXPECT_FLOAT_EQ(rotationY[1][3], 0);

        EXPECT_FLOAT_EQ(rotationY[2][0], -s);
        EXPECT_FLOAT_EQ(rotationY[2][1], 0);
        EXPECT_FLOAT_EQ(rotationY[2][2], c);
        EXPECT_FLOAT_EQ(rotationY[2][3], 0);

        EXPECT_FLOAT_EQ(rotationY[3][0], 0);
        EXPECT_FLOAT_EQ(rotationY[3][1], 0);
        EXPECT_FLOAT_EQ(rotationY[3][2], 0);
        EXPECT_FLOAT_EQ(rotationY[3][3], 1);
    }

    //Z
    {
        mat4f const rotationZ{ rotate(angle, z) };

        EXPECT_FLOAT_EQ(rotationZ[0][0], c);
        EXPECT_FLOAT_EQ(rotationZ[0][1], -s);
        EXPECT_FLOAT_EQ(rotationZ[0][2], 0);
        EXPECT_FLOAT_EQ(rotationZ[0][3], 0);

        EXPECT_FLOAT_EQ(rotationZ[1][0], s);
        EXPECT_FLOAT_EQ(rotationZ[1][1], c);
        EXPECT_FLOAT_EQ(rotationZ[1][2], 0);
        EXPECT_FLOAT_EQ(rotationZ[1][3], 0);

        EXPECT_FLOAT_EQ(rotationZ[2][0], 0);
        EXPECT_FLOAT_EQ(rotationZ[2][1], 0);
        EXPECT_FLOAT_EQ(rotationZ[2][2], 1);
        EXPECT_FLOAT_EQ(rotationZ[2][3], 0);

        EXPECT_FLOAT_EQ(rotationZ[3][0], 0);
        EXPECT_FLOAT_EQ(rotationZ[3][1], 0);
        EXPECT_FLOAT_EQ(rotationZ[3][2], 0);
        EXPECT_FLOAT_EQ(rotationZ[3][3], 1);
    }
}

TEST(matrix_tests, can_rotate_a_matrix_euler_angle) {
    float const angle{ 45.0f * 0.0174533f };

    vec3f const x{ angle, 0, 0 };
    vec3f const y{ 0, angle, 0 };
    vec3f const z{ 0, 0, angle };

    float const c{ std::cos(angle) };
    float const s{ std::sin(angle) };

    //X
    {
        mat4f const rotationX{ rotate(x) };

        EXPECT_FLOAT_EQ(rotationX[0][0], 1);
        EXPECT_FLOAT_EQ(rotationX[0][1], 0);
        EXPECT_FLOAT_EQ(rotationX[0][2], 0);
        EXPECT_FLOAT_EQ(rotationX[0][3], 0);

        EXPECT_FLOAT_EQ(rotationX[1][0], 0);
        EXPECT_FLOAT_EQ(rotationX[1][1], c);
        EXPECT_FLOAT_EQ(rotationX[1][2], s);
        EXPECT_FLOAT_EQ(rotationX[1][3], 0);

        EXPECT_FLOAT_EQ(rotationX[2][0], 0);
        EXPECT_FLOAT_EQ(rotationX[2][1], -s);
        EXPECT_FLOAT_EQ(rotationX[2][2], c);
        EXPECT_FLOAT_EQ(rotationX[2][3], 0);

        EXPECT_FLOAT_EQ(rotationX[3][0], 0);
        EXPECT_FLOAT_EQ(rotationX[3][1], 0);
        EXPECT_FLOAT_EQ(rotationX[3][2], 0);
        EXPECT_FLOAT_EQ(rotationX[3][3], 1);
    }

    //Y
    {
        mat4f const rotationY{ rotate(y) };

        EXPECT_FLOAT_EQ(rotationY[0][0], c);
        EXPECT_FLOAT_EQ(rotationY[0][1], 0);
        EXPECT_FLOAT_EQ(rotationY[0][2], -s);
        EXPECT_FLOAT_EQ(rotationY[0][3], 0);

        EXPECT_FLOAT_EQ(rotationY[1][0], 0);
        EXPECT_FLOAT_EQ(rotationY[1][1], 1);
        EXPECT_FLOAT_EQ(rotationY[1][2], 0);
        EXPECT_FLOAT_EQ(rotationY[1][3], 0);

        EXPECT_FLOAT_EQ(rotationY[2][0], s);
        EXPECT_FLOAT_EQ(rotationY[2][1], 0);
        EXPECT_FLOAT_EQ(rotationY[2][2], c);
        EXPECT_FLOAT_EQ(rotationY[2][3], 0);

        EXPECT_FLOAT_EQ(rotationY[3][0], 0);
        EXPECT_FLOAT_EQ(rotationY[3][1], 0);
        EXPECT_FLOAT_EQ(rotationY[3][2], 0);
        EXPECT_FLOAT_EQ(rotationY[3][3], 1);
    }

    //Z
    {
        mat4f const rotationZ{ rotate(z) };

        EXPECT_FLOAT_EQ(rotationZ[0][0], c);
        EXPECT_FLOAT_EQ(rotationZ[0][1], s);
        EXPECT_FLOAT_EQ(rotationZ[0][2], 0);
        EXPECT_FLOAT_EQ(rotationZ[0][3], 0);

        EXPECT_FLOAT_EQ(rotationZ[1][0], -s);
        EXPECT_FLOAT_EQ(rotationZ[1][1], c);
        EXPECT_FLOAT_EQ(rotationZ[1][2], 0);
        EXPECT_FLOAT_EQ(rotationZ[1][3], 0);

        EXPECT_FLOAT_EQ(rotationZ[2][0], 0);
        EXPECT_FLOAT_EQ(rotationZ[2][1], 0);
        EXPECT_FLOAT_EQ(rotationZ[2][2], 1);
        EXPECT_FLOAT_EQ(rotationZ[2][3], 0);

        EXPECT_FLOAT_EQ(rotationZ[3][0], 0);
        EXPECT_FLOAT_EQ(rotationZ[3][1], 0);
        EXPECT_FLOAT_EQ(rotationZ[3][2], 0);
        EXPECT_FLOAT_EQ(rotationZ[3][3], 1);
    }
}

TEST(matrix_tests, can_scale_a_matrix) {
    vec3f const v{ 8, 4, 3 };

    mat4f const scaling{ scale(v) };

    EXPECT_EQ(scaling[0][0], 8);
    EXPECT_EQ(scaling[0][1], 0);
    EXPECT_EQ(scaling[0][2], 0);
    EXPECT_EQ(scaling[0][3], 0);

    EXPECT_EQ(scaling[1][0], 0);
    EXPECT_EQ(scaling[1][1], 4);
    EXPECT_EQ(scaling[1][2], 0);
    EXPECT_EQ(scaling[1][3], 0);

    EXPECT_EQ(scaling[2][0], 0);
    EXPECT_EQ(scaling[2][1], 0);
    EXPECT_EQ(scaling[2][2], 3);
    EXPECT_EQ(scaling[2][3], 0);

    EXPECT_EQ(scaling[3][0], 0);
    EXPECT_EQ(scaling[3][1], 0);
    EXPECT_EQ(scaling[3][2], 0);
    EXPECT_EQ(scaling[3][3], 1);
}