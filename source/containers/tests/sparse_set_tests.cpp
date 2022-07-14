#include <ember/containers/sparse_set.hpp>
#include <gtest/gtest.h>

using namespace ember;

struct object {
    std::int32_t a{};
    std::int32_t b{};
    std::int32_t c{};

    friend auto operator<=>(object const &lhs, object const &rhs) = default;
};

TEST(sparse_set_tests, can_default_initialise) {
    sparse_set<std::uint32_t, object> set{};

    EXPECT_EQ(set.size(), 0);
}

TEST(sparse_set_tests, can_index_into_set) {
    std::size_t constexpr set_size{ 8 };
    sparse_set<std::size_t, object> set{};

    for(std::size_t i{ 0 }; i < set_size; ++i) {
        auto val{ static_cast<std::int32_t>(i) };
        set[i] = object{ val, val + 1, val + 2 };
    }

    EXPECT_EQ(set[2], object(2, 2 + 1, 2 + 2));
    EXPECT_EQ(set[1], object(1, 1 + 1, 1 + 2));
    EXPECT_EQ(set[7], object(7, 7 + 1, 7 + 2));
    EXPECT_EQ(set[4], object(4, 4 + 1, 4 + 2));
    EXPECT_EQ(set[6], object(6, 6 + 1, 6 + 2));
    EXPECT_EQ(set[0], object(0, 0 + 1, 0 + 2));
    EXPECT_EQ(set[5], object(5, 5 + 1, 5 + 2));
    EXPECT_EQ(set[3], object(3, 3 + 1, 3 + 2));
}

TEST(sparse_set_tests, can_iterate_over_set) {
    std::size_t constexpr set_size{ 24 };
    sparse_set<std::size_t, object> set{};

    for(std::size_t i{ 0 }; i < set_size; ++i) {
        auto val{ static_cast<std::int32_t>(i) };

        set[i].a = val;
        set[i].b = val;
        set[i].c = val;
    }

    std::int32_t index{ 0 };

    for(object &obj : set) {
        object val{ index, index, index };
        EXPECT_EQ(obj, val);

        obj.a *= 2;
        obj.b *= 2;
        obj.c *= 2;

        ++index;
    }
    index = 0;

    for(auto iter{ set.begin() }; iter != set.end(); ++iter) {
        object val{ index * 2, index * 2, index * 2 };
        EXPECT_EQ(*iter, val);

        ++index;
    }
}

TEST(sprase_set_tests, can_add_new_items_to_set) {
    sparse_set<std::size_t, object> set{};

    set.emplace(32, 1, 2, 3);
    set.emplace(1);
    set.emplace(10000, 0, 0, 1);

    EXPECT_TRUE(set.contains(32));
    EXPECT_TRUE(set.contains(1));
    EXPECT_TRUE(set.contains(10000));

    EXPECT_FALSE(set.contains(0));
    EXPECT_FALSE(set.contains(9));
    EXPECT_FALSE(set.contains(1234));

    EXPECT_EQ(set[32].a, 1);
    EXPECT_EQ(set[32].b, 2);
    EXPECT_EQ(set[32].c, 3);
    EXPECT_EQ(set[1].a, 0);
    EXPECT_EQ(set[1].b, 0);
    EXPECT_EQ(set[1].c, 0);
    EXPECT_EQ(set[10000].a, 0);
    EXPECT_EQ(set[10000].b, 0);
    EXPECT_EQ(set[10000].c, 1);
}

TEST(sprase_set_tests, can_erase_items) {
    sparse_set<std::size_t, object> set{};

    set.emplace(32, 1, 2, 3);
    set.emplace(1, 8, 8, 8);
    set.emplace(10000, 0, 0, 1);

    ASSERT_TRUE(set.contains(32));
    ASSERT_TRUE(set.contains(1));
    ASSERT_TRUE(set.contains(10000));

    EXPECT_EQ(set[32], object(1, 2, 3));
    EXPECT_EQ(set[1], object(8, 8, 8));
    EXPECT_EQ(set[10000], object(0, 0, 1));
    EXPECT_EQ(set.size(), 3);

    set.erase(1);

    EXPECT_TRUE(set.contains(32));
    EXPECT_FALSE(set.contains(1));
    EXPECT_TRUE(set.contains(10000));
    EXPECT_EQ(set.size(), 2);

    EXPECT_EQ(set[32], object(1, 2, 3));
    EXPECT_EQ(set[10000], object(0, 0, 1));

    set.erase(10000);

    EXPECT_TRUE(set.contains(32));
    EXPECT_FALSE(set.contains(1));
    EXPECT_FALSE(set.contains(10000));
    EXPECT_EQ(set.size(), 1);

    EXPECT_EQ(set[32], object(1, 2, 3));

    set.erase(32);

    EXPECT_FALSE(set.contains(32));
    EXPECT_FALSE(set.contains(1));
    EXPECT_FALSE(set.contains(10000));
    EXPECT_EQ(set.size(), 0);
}

static std::uint32_t copy_counter{ 0 };
TEST(sprase_set_tests, can_copy) {
    struct helper_type {
        ~helper_type() {
            ++copy_counter;
        };
    };

    sparse_set<std::size_t, helper_type> set_1{};
    sparse_set<std::size_t, helper_type> set_2{};

    set_1.emplace(5);
    set_1.emplace(6);
    set_1.emplace(7);

    set_2.emplace(1);
    set_2.emplace(2);

    copy_counter = 0;//dtor is called when reallocating

    set_2 = set_1;

    EXPECT_EQ(copy_counter, 2);
    EXPECT_EQ(set_2.size(), set_1.size());
    EXPECT_TRUE(set_2.contains(5));
    EXPECT_TRUE(set_2.contains(6));
    EXPECT_TRUE(set_2.contains(7));
    EXPECT_FALSE(set_2.contains(1));
    EXPECT_FALSE(set_2.contains(2));
}

static std::uint32_t move_counter{ 0 };
TEST(sprase_set_tests, can_move) {
    struct helper_type {
        ~helper_type() {
            ++move_counter;
        };
    };

    sparse_set<std::size_t, helper_type> set_1{};
    sparse_set<std::size_t, helper_type> set_2{};

    set_1.emplace(5);
    set_1.emplace(6);
    set_1.emplace(7);

    set_2.emplace(1);
    set_2.emplace(2);

    std::size_t size{ set_1.size() };

    move_counter = 0;//dtor is called when reallocating

    set_2 = std::move(set_1);

    EXPECT_EQ(move_counter, 2);
    EXPECT_EQ(set_2.size(), size);
    EXPECT_TRUE(set_2.contains(5));
    EXPECT_TRUE(set_2.contains(6));
    EXPECT_TRUE(set_2.contains(7));
    EXPECT_FALSE(set_2.contains(1));
    EXPECT_FALSE(set_2.contains(2));
}

static std::uint32_t clear_counter{ 0 };
TEST(sprase_set_tests, can_clear) {
    struct destructor_helper {
        ~destructor_helper() {
            ++clear_counter;
        };
    };

    sparse_set<std::size_t, destructor_helper> set{};

    set.emplace(1);
    set.emplace(2);
    set.emplace(3);

    clear_counter = 0;//Reset count as dtors are called when resizing now

    ASSERT_EQ(set.size(), 3);

    set.clear();

    EXPECT_EQ(set.size(), 0);
    EXPECT_EQ(clear_counter, 3);
}
