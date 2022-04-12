#include <cstddef>
#include <ember/memory/memory.hpp>
#include <gtest/gtest.h>
#include <new>

using namespace ember;

TEST(allocation_tests, can_allocate_memory) {
    float *mem_1{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };
    float *mem_2{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

    EXPECT_NE(mem_1, nullptr);
    EXPECT_NE(mem_2, nullptr);

    *mem_1 = 1.0f;
    *mem_2 = 2.0f;

    EXPECT_NE(mem_1, mem_2);
}

TEST(allocation_tests, can_free_memory) {
    {
        auto *mem_1{ memory::alloc(100, 0) };
        auto *mem_2{ memory::alloc(999, 0) };

        ASSERT_NE(mem_1, nullptr);
        ASSERT_NE(mem_2, nullptr);

        memory::free(mem_1);
        memory::free(mem_2);

        EXPECT_EQ(mem_1, nullptr);
        EXPECT_EQ(mem_2, nullptr);
    }

    {
        auto *mem_1{ memory::alloc(100, 0) };
        auto *mem_2{ memory::alloc(999, 0) };

        ASSERT_NE(mem_1, nullptr);
        ASSERT_NE(mem_2, nullptr);

        memory::free(mem_2);
        memory::free(mem_1);

        EXPECT_EQ(mem_1, nullptr);
        EXPECT_EQ(mem_2, nullptr);
    }
}

TEST(allocation_tests, can_reallocate_memory) {
    auto *mem_1{ memory::alloc(200, alignof(float)) };

    *reinterpret_cast<float *>(mem_1) = 1.0f;

    ASSERT_EQ(*reinterpret_cast<float *>(mem_1), 1.0f);

    mem_1 = memory::realloc(mem_1, 400, alignof(float));

    ASSERT_NE(mem_1, nullptr);
    EXPECT_EQ(*reinterpret_cast<float *>(mem_1), 1.0f);
}

TEST(allocation_tests, can_allocate_large_amounts_of_memory) {
    {
        std::byte *large_1{ memory::alloc(EMBER_MB(500), 0) };
        float *mem_1{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

        EXPECT_NE(large_1, nullptr);
        EXPECT_NE(mem_1, nullptr);

        *mem_1 = 1.0f;

        EXPECT_EQ(*mem_1, 1.0f);
    }

    {
        std::byte *large_2{ memory::alloc(EMBER_GB(2), 0) };
        float *mem_2{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

        EXPECT_NE(large_2, nullptr);
        EXPECT_NE(mem_2, nullptr);

        *mem_2 = 2.0f;

        EXPECT_EQ(*mem_2, 2.0f);
    }

    {
        std::byte *large_3{ memory::alloc(EMBER_GB(6), 0) };
        float *mem_3{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

        EXPECT_NE(large_3, nullptr);
        EXPECT_NE(mem_3, nullptr);

        *mem_3 = 3.0f;

        EXPECT_EQ(*mem_3, 3.0f);
    }
}

TEST(allocation_tests, can_construct_objects) {
    struct test_type {
        std::uint32_t a{ 0 };
        std::uint64_t b{ 1 };
        float c{ 2.0f };
        double d{ 3.0f };

        test_type(std::uint32_t a)
            : a{ a } {
        }
    };

    test_type *type{ memory::construct<test_type>(3u) };

    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->a, 3);
    EXPECT_EQ(type->b, 1);
    EXPECT_EQ(type->c, 2.0f);
    EXPECT_EQ(type->d, 3.0f);
}

TEST(allocation_tests, can_destruct_objects) {
    struct test_type {
        bool &dtor_called;

        test_type(bool &dtor_called)
            : dtor_called{ dtor_called } {
        }
        ~test_type() {
            dtor_called = true;
        }
    };

    bool dtor_called{ false };

    test_type *type{ memory::construct<test_type>(dtor_called) };

    ASSERT_FALSE(dtor_called);

    memory::destruct(type);

    EXPECT_TRUE(dtor_called);
    EXPECT_EQ(type, nullptr);
}