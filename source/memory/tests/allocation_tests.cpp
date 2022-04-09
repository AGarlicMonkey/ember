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
    auto *mem{ memory::alloc(100, 0) };

    ASSERT_NE(mem, nullptr);

    memory::free(mem);

    EXPECT_EQ(mem, nullptr);
}