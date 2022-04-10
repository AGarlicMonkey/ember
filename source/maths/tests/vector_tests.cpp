#include <ember/maths/vector.hpp>
#include <gtest/gtest.h>

using namespace ember::maths;

TEST(vector_tests, can_default_construct_a_vector) {
    vec<2, float> v2{};

    EXPECT_EQ(v2.x, float{});
    EXPECT_EQ(v2.r, float{});
    EXPECT_EQ(v2.u, float{});

    EXPECT_EQ(v2.y, float{});
    EXPECT_EQ(v2.g, float{});
    EXPECT_EQ(v2.v, float{});

    vec<3, float> v3{};

    EXPECT_EQ(v3.x, float{});
    EXPECT_EQ(v3.r, float{});

    EXPECT_EQ(v3.y, float{});
    EXPECT_EQ(v3.g, float{});

    EXPECT_EQ(v3.z, float{});
    EXPECT_EQ(v3.b, float{});

    vec<4, float> v4{};

    EXPECT_EQ(v4.x, float{});
    EXPECT_EQ(v4.r, float{});

    EXPECT_EQ(v4.y, float{});
    EXPECT_EQ(v4.g, float{});

    EXPECT_EQ(v4.z, float{});
    EXPECT_EQ(v4.b, float{});

    EXPECT_EQ(v4.w, float{});
    EXPECT_EQ(v4.a, float{});
}

TEST(vector_tests, can_access_members_by_index) {
    vec2f v2{ 0, 1 };

    EXPECT_EQ(v2.x, v2[0]);
    EXPECT_EQ(v2.y, v2[1]);

    EXPECT_EQ(v2.y, v2[2]);

    vec3f v3{ 0, 1, 2 };

    EXPECT_EQ(v3.x, v3[0]);
    EXPECT_EQ(v3.y, v3[1]);
    EXPECT_EQ(v3.z, v3[2]);

    EXPECT_EQ(v3.z, v3[3]);

    vec4f v4{ 0, 1, 2, 3 };

    EXPECT_EQ(v4.x, v4[0]);
    EXPECT_EQ(v4.y, v4[1]);
    EXPECT_EQ(v4.z, v4[2]);
    EXPECT_EQ(v4.w, v4[3]);

    EXPECT_EQ(v4.w, v4[4]);
}

TEST(vector_tests, can_compare_two_vectors){
    vec2f v2a{ 1, 2 };
    vec2f v2b{ 1, 2 };
    vec2f v2c{ 4, 3 };

    EXPECT_TRUE(v2a == v2b);
    EXPECT_TRUE(v2b != v2c);

    EXPECT_FALSE(v2a == v2c);
    EXPECT_FALSE(v2a != v2b);

    vec3f v3a{ 2, 2, 2 };
    vec3f v3b{ 2, 2, 2 };
    vec3f v3c{ 4, 6, 7 };

    EXPECT_TRUE(v3a == v3b);
    EXPECT_TRUE(v3b != v3c);

    EXPECT_FALSE(v3a == v3c);
    EXPECT_FALSE(v3a != v3b);

    vec4f v4a{ 3, 2, 1, 6 };
    vec4f v4b{ 3, 2, 1, 6 };
    vec4f v4c{ 9, 44, 5, 2 };

    EXPECT_TRUE(v4a == v4b);
    EXPECT_TRUE(v4b != v4c);

    EXPECT_FALSE(v4a == v4c);
    EXPECT_FALSE(v4a != v4b);
}

TEST(vector_tests, can_add_two_vectors) {
    vec2f v2a{ 1, 2 };
    vec2f v2b{ 1, 2 };
    vec2f v2c{ v2a };

    vec2f const v2sum{ v2a.x + v2b.x, v2a.y + v2b.y };
    EXPECT_EQ(v2a + v2b, v2sum);

    v2c += v2b;

    EXPECT_EQ(v2c.x, v2a.x + v2b.x);
    EXPECT_EQ(v2c.y, v2a.y + v2b.y);

    vec3f v3a{ 2, 2, 2 };
    vec3f v3b{ 2, 2, 2 };
    vec3f v3c{ v3a };

    vec3f const v3sum{ v3a.x + v3b.x, v3a.y + v3b.y, v3a.z + v3b.z };
    EXPECT_EQ(v3a + v3b, v3sum);

    v3c += v3b;

    EXPECT_EQ(v3c.x, v3a.x + v3b.x);
    EXPECT_EQ(v3c.y, v3a.y + v3b.y);
    EXPECT_EQ(v3c.z, v3a.z + v3b.z);

    vec4f v4a{ 3, 2, 1, 6 };
    vec4f v4b{ 2, 2, 2 };
    vec4f v4c{ v4a };

    vec4f const v4sum{ v4a.x + v4b.x, v4a.y + v4b.y, v4a.z + v4b.z, v4a.w + v4b.w };
    EXPECT_EQ(v4a + v4b, v4sum);

    v4c += v4b;

    EXPECT_EQ(v4c.x, v4a.x + v4b.x);
    EXPECT_EQ(v4c.y, v4a.y + v4b.y);
    EXPECT_EQ(v4c.z, v4a.z + v4b.z);
    EXPECT_EQ(v4c.w, v4a.w + v4b.w);
}

TEST(vector_tests, can_subtract_two_vectors) {
    {
        vec2f a{ 1, 2 };
        vec2f b{ 1, 2 };
        vec2f c{ a };

        vec2f const sub{ a.x - b.x, a.y - b.y };
        EXPECT_EQ(a - b, sub);

        c -= b;
        EXPECT_EQ(c.x, a.x - b.x);
        EXPECT_EQ(c.y, a.y - b.y);
    }

    {
        vec3f a{ 2, 2, 2 };
        vec3f b{ 2, 2, 2 };
        vec3f c{ a };

        vec3f const sub{ a.x - b.x, a.y - b.y, a.z - b.z };
        EXPECT_EQ(a - b, sub);

        c -= b;
        EXPECT_EQ(c.x, a.x - b.x);
        EXPECT_EQ(c.y, a.y - b.y);
        EXPECT_EQ(c.z, a.z - b.z);
    }

    {
        vec4f a{ 3, 2, 1, 6 };
        vec4f b{ 2, 2, 2 };
        vec4f c{ a };

        vec4f const sub{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
        EXPECT_EQ(a - b, sub);

        c -= b;
        EXPECT_EQ(c.x, a.x - b.x);
        EXPECT_EQ(c.y, a.y - b.y);
        EXPECT_EQ(c.z, a.z - b.z);
        EXPECT_EQ(c.w, a.w - b.w);
    }
}

TEST(vector_tests, can_multiply_vector_by_number) {
    float constexpr multiplier{ 5.0f };

    {
        vec2f a{ 2, 5 };
        vec2f aMul{ a.x * multiplier, a.y * multiplier };
        EXPECT_EQ(a * multiplier, aMul);

        vec2f b{ 1, 3 };
        vec2f c{ b };

        c *= multiplier;
        EXPECT_EQ(b.x * multiplier, c.x);
        EXPECT_EQ(b.y * multiplier, c.y);
    }

    {
        vec3f a{ 2, 5, 8 };
        vec3f aMul{ a.x * multiplier, a.y * multiplier, a.z * multiplier };
        EXPECT_EQ(a * multiplier, aMul);

        vec3f b{ 1, 3, 2 };
        vec3f c{ b };

        c *= multiplier;
        EXPECT_EQ(b.x * multiplier, c.x);
        EXPECT_EQ(b.y * multiplier, c.y);
        EXPECT_EQ(b.z * multiplier, c.z);
    }

    {
        vec4f a{ 2, 5, 8, 1};
        vec4f aMul{ a.x * multiplier, a.y * multiplier, a.z * multiplier, a.w * multiplier };
        EXPECT_EQ(a * multiplier, aMul);

        vec4f b{ 1, 3, 2, 0 };
        vec4f c{ b };

        c *= multiplier;
        EXPECT_EQ(b.x * multiplier, c.x);
        EXPECT_EQ(b.y * multiplier, c.y);
        EXPECT_EQ(b.z * multiplier, c.z);
        EXPECT_EQ(b.w * multiplier, c.w);
    }
}

TEST(vector_tests, can_divide_vector_by_number) {
    float constexpr divisor{ 3.0f };

    vec2f v2a{ 3, 2 };
    vec2f v2b{ 1, 2 };
    vec2f v2c{ v2b };

    vec2f v2Div{ v2a.x / divisor, v2b.y / divisor };
    EXPECT_EQ(v2a / divisor, v2Div);

    v2b /= divisor;
    EXPECT_EQ(v2b.x, v2c.x / divisor);
    EXPECT_EQ(v2b.y, v2c.y / divisor);

    vec3f v3a{ 2, 2, 2 };
    vec3f v3b{ 1, 2, 3 };
    vec3f v3c{ v3b };

    vec3f v3Div{ v3a.x / divisor, v3a.y / divisor, v3a.z / divisor };
    EXPECT_EQ(v3a / divisor, v3Div);

    v3b /= divisor;
    EXPECT_EQ(v3b.x, v3c.x / divisor);
    EXPECT_EQ(v3b.y, v3c.y / divisor);
    EXPECT_EQ(v3b.z, v3c.z / divisor);

    vec4f v4a{ 3, 2, 1, 6 };
    vec4f v4b{ 2, 1, 3, 6 };
    vec4f v4c{ v4b };

    vec4f v4Div{ v4a.x / divisor, v4a.y / divisor, v4a.z / divisor, v4a.w / divisor };
    EXPECT_EQ(v4a / divisor, v4Div);

    v4b /= divisor;
    EXPECT_EQ(v4b.x, v4c.x / divisor);
    EXPECT_EQ(v4b.y, v4c.y / divisor);
    EXPECT_EQ(v4b.z, v4c.z / divisor);
    EXPECT_EQ(v4b.w, v4c.w / divisor);
}

TEST(vector_tests, can_get_length_of_a_vector) {
    vec2f v2{ 1, 2 };

    EXPECT_EQ(length(v2), std::sqrt(v2.x * v2.x + v2.y * v2.y));

    vec3f v3{ 2, 2, 2 };

    EXPECT_EQ(length(v3), std::sqrt(v3.x * v3.x + v3.y * v3.y + v3.z * v3.z));

    vec4f v4{ 3, 2, 1, 6 };

    EXPECT_EQ(length(v4), std::sqrt(v4.x * v4.x + v4.y * v4.y + v4.z * v4.z + v4.w * v4.w));
}

TEST(vector_tests, can_get_distance_between_two_vectors) {
    {
        vec2f a{ 1, 5 };
        vec2f b{ 3, 8 };

        float result{ static_cast<float>(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2))) };
        float dist{ distance(a, b) };

        EXPECT_EQ(dist, result);
        EXPECT_GE(dist, 0.0f);
    }

    {
        vec3f a{ 3, 3, 8 };
        vec3f b{ 9, 1, 9 };

        float result{ static_cast<float>(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2))) };
        float dist{ distance(a, b) };

        EXPECT_EQ(dist, result);
        EXPECT_GE(dist, 0.0f);
    }

    {
        vec4f a{ 0, 4, 6, 7 };
        vec4f b{ 1, 2, 3, 4 };

        float result{ static_cast<float>(std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2) + std::pow(a.w - b.w, 2))) };
        float dist{ distance(a, b) };

        EXPECT_EQ(dist, result);
        EXPECT_GE(dist, 0.0f);
    }
}

TEST(vector_tests, can_normalise_a_vector) {
    float constexpr boundsError{ 0.00001f };

    vec2f v2{ 3, 2 };

    EXPECT_NEAR(length(normalise(v2)), 1.0f, boundsError);

    vec3f v3{ 2, 1, 7 };

    EXPECT_NEAR(length(normalise(v3)), 1.0f, boundsError);

    vec4f v4{ 1, 4, 7, 3 };

    EXPECT_NEAR(length(normalise(v4)), 1.0f, boundsError);
}

TEST(vector_tests, can_dot_product_two_vectors) {
    float constexpr boundsError{ 0.00001f };

    vec2f v2a{ 3, 2 };
    vec2f v2b{ 1, 5 };

    const float v2Dot{ dot(v2a, v2b) };
    const float v2DotTest{ v2a.x * v2b.x + v2a.y * v2b.y };
    EXPECT_NEAR(v2Dot, v2DotTest, boundsError);

    vec3f v3a{ 2, 1, 7 };
    vec3f v3b{ 3, 10, 1 };

    const float v3Dot{ dot(v3a, v3b) };
    const float v3DotTest{ v3a.x * v3b.x + v3a.y * v3b.y + v3a.z * v3b.z };
    EXPECT_NEAR(v3Dot, v3DotTest, boundsError);

    vec4f v4a{ 1, 4, 7, 3 };
    vec4f v4b{ 3, 3, 3, 3 };

    const float v4Dot{ dot(v4a, v4b) };
    const float v4DotTest{ v4a.x * v4b.x + v4a.y * v4b.y + v4a.z * v4b.z + v4a.w * v4b.w };
    EXPECT_NEAR(v4Dot, v4DotTest, boundsError);
}

TEST(vector_tests, can_cross_product_two_vectors) {
    vec3f v3a{ 2, 7, 3 };
    vec3f v3b{ 3, 8, 1 };

    const vec3f v3Cross{ cross(v3a, v3b) };
    const vec3f v3CrossTest{
        .x = (v3a.y * v3b.z) - (v3a.z * v3b.y),
        .y = (v3a.z * v3b.x) - (v3a.x * v3b.z),
        .z = (v3a.x * v3b.y) - (v3a.y * v3b.x),
    };
    EXPECT_EQ(v3Cross, v3CrossTest);
}

TEST(vector_tests, can_lerp_two_vectors) {
    {
        vec2f const a{ 0.0f, 0.0f };
        vec2f const b{ 1.0f, 1.0f };

        vec2f res0{ lerp(a, b, 0.0f) };
        EXPECT_EQ(res0, a);

        vec2f res1{ lerp(a, b, 1.0f) };
        EXPECT_EQ(res1, b);

        vec2f res2{ lerp(a, b, 0.5f) };
        EXPECT_EQ(res2.x, 0.5f);
        EXPECT_EQ(res2.y, 0.5f);
    }

    {
        vec3f const a{ 0.0f, 0.0f, 0.0f };
        vec3f const b{ 1.0f, 1.0f, 1.0f };

        vec3f res0{ lerp(a, b, 0.0f) };
        EXPECT_EQ(res0, a);

        vec3f res1{ lerp(a, b, 1.0f) };
        EXPECT_EQ(res1, b);

        vec3f res2{ lerp(a, b, 0.5f) };
        EXPECT_EQ(res2.x, 0.5f);
        EXPECT_EQ(res2.y, 0.5f);
        EXPECT_EQ(res2.z, 0.5f);
    }

    {
        vec4f const a{ 0.0f, 0.0f, 0.0f, 0.0f };
        vec4f const b{ 1.0f, 1.0f, 1.0f, 1.0f };

        vec4f res0{ lerp(a, b, 0.0f) };
        EXPECT_EQ(res0, a);

        vec4f res1{ lerp(a, b, 1.0f) };
        EXPECT_EQ(res1, b);

        vec4f res2{ lerp(a, b, 0.5f) };
        EXPECT_EQ(res2.x, 0.5f);
        EXPECT_EQ(res2.y, 0.5f);
        EXPECT_EQ(res2.z, 0.5f);
        EXPECT_EQ(res2.w, 0.5f);
    }
}