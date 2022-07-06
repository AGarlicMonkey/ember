#include <ember/memory/unique_ptr.hpp>
#include <gtest/gtest.h>

using namespace ember::memory;

TEST(unique_ptr_tests, does_not_create_nullptr) {
    unique_ptr<float> ptr{ make_unique<float>() };

    EXPECT_NE(ptr, nullptr);
}

TEST(unique_ptr_tests, can_be_initialised_to_nullptr) {
    unique_ptr<float> ptr{ nullptr };

    EXPECT_EQ(ptr, nullptr);
}

TEST(unique_ptr_tests, can_access_object) {
    struct test_type {
        std::uint32_t a;
    };

    unique_ptr<test_type> ptr{ make_unique<test_type>() };

    ptr->a = 1;
    *ptr   = test_type{ 3 };

    EXPECT_EQ(ptr->a, 3);
}

TEST(unique_ptr_tests, constructor_is_properly_called) {
    struct test_type {
        std::uint32_t a;

        test_type(std::uint32_t val)
            : a{ val } {
        }
    };

    unique_ptr<test_type> ptr{ make_unique<test_type>(5u) };

    EXPECT_EQ(ptr->a, 5);
}

TEST(unique_ptr_tests, destructor_is_properly_called) {
    struct test_type {
        bool &called;

        test_type(bool &called)
            : called{ called } {
        }
        ~test_type() {
            called = true;
        }
    };

    bool reset_called{ false };
    unique_ptr<test_type> reset_ptr{ make_unique<test_type>(reset_called) };

    ASSERT_EQ(reset_called, false);

    reset_ptr.reset();

    EXPECT_EQ(reset_called, true);

    bool scope_called{ false };
    {
        unique_ptr<test_type> reset_ptr{ make_unique<test_type>(scope_called) };

        ASSERT_EQ(scope_called, false);
    }

    EXPECT_EQ(scope_called, true);
}

TEST(unique_ptr_tests, ownership_is_properly_transfered) {
    unique_ptr<std::int32_t> a{ make_unique<std::int32_t>(5) };
    unique_ptr<std::int32_t> b{};

    ASSERT_NE(a, nullptr);
    ASSERT_EQ(*a, 5);
    ASSERT_EQ(b, nullptr);

    b = std::move(a);

    EXPECT_EQ(a, nullptr);
    EXPECT_NE(b, nullptr);
    EXPECT_EQ(*b, 5);
}

TEST(unique_ptr_tests, prior_data_is_destroyed_when_transfering_ownership) {
    struct test_type {
        bool &called;

        test_type(bool &called)
            : called{ called } {
        }
        ~test_type() {
            called = true;
        }
    };

    bool a_called{ false };
    bool b_called{ false };

    unique_ptr<test_type> a{ make_unique<test_type>(a_called) };
    unique_ptr<test_type> b{ make_unique<test_type>(b_called) };

    EXPECT_FALSE(a_called);
    EXPECT_FALSE(b_called);

    a = std::move(b);

    EXPECT_TRUE(a_called);
    EXPECT_FALSE(b_called);
}