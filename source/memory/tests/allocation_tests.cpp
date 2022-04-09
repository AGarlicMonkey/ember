#include <ember/memory/memory.hpp>
#include <gtest/gtest.h>

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

TEST(allocation_tests, can_free_memory){
    auto *mem_1{ memory::alloc(100, 0) };
    auto *mem_2{ memory::alloc(999, 0) };

    ASSERT_NE(mem_1, nullptr);
    ASSERT_NE(mem_2, nullptr);

    memory::free(mem_1);
    memory::free(mem_2);

    EXPECT_EQ(mem_1, nullptr);
    EXPECT_EQ(mem_2, nullptr);
}

TEST(allocation_tests, can_allocate_large_amounts_of_memory) {
    std::byte *large_1{ memory::alloc(EMBER_MB(500), 0) };
    float *mem_1{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

    EXPECT_NE(large_1, nullptr);
    EXPECT_NE(mem_1, nullptr);

    *mem_1 = 1.0f;

    EXPECT_EQ(*mem_1, 1.0f);

    std::byte *large_2{ memory::alloc(EMBER_GB(2), 0) };
    float *mem_2{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

    EXPECT_NE(large_2, nullptr);
    EXPECT_NE(mem_2, nullptr);

    *mem_2 = 2.0f;

    EXPECT_EQ(*mem_2, 2.0f);

    std::byte *large_3{ memory::alloc(EMBER_GB(6), 0) };
    float *mem_3{ reinterpret_cast<float *>(memory::alloc(sizeof(float), alignof(float))) };

    EXPECT_NE(large_3, nullptr);
    EXPECT_NE(mem_3, nullptr);

    *mem_3 = 3.0f;

    EXPECT_EQ(*mem_3, 3.0f);
}