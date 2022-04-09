#include <ember/memory/memory.hpp>
#include <gtest/gtest.h>

using namespace ember;

TEST(allocation_tests, can_allocate_memory) {
    auto handle_1{ memory::alloc(100, 0) };

    EXPECT_TRUE(handle_1.is_valid());
    EXPECT_NE(handle_1.get(), nullptr);

    auto handle_2{ memory::alloc(100, 0) };

    EXPECT_TRUE(handle_2.is_valid());
    EXPECT_NE(handle_2.get(), nullptr);
}