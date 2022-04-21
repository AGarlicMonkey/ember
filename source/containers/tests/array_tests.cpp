#include <ember/containers/array.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace ember::containers;

TEST(array_tests, can_default_initialise) {
    array<std::uint32_t> arr{};

    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 0);
}

TEST(array_tests, can_size_initialise) {
    std::size_t constexpr array_size{ 8 };

    array<std::uint32_t> arr(array_size);

    EXPECT_EQ(arr.size(), array_size);
    EXPECT_EQ(arr.capacity(), array_size);
}

TEST(array_tests, can_index_into_array) {
    std::size_t constexpr array_size{ 8 };

    array<std::uint32_t> arr(array_size);

    for(std::size_t i{ 0 }; i < array_size; ++i) {
        EXPECT_EQ(arr[i], 0);
    }

    for(std::size_t i{ 0 }; i < array_size; ++i) {
        arr[i] = i;
    }

    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[1], 1);
    EXPECT_EQ(arr[2], 2);
    EXPECT_EQ(arr[3], 3);
    EXPECT_EQ(arr[4], 4);
    EXPECT_EQ(arr[5], 5);
    EXPECT_EQ(arr[6], 6);
    EXPECT_EQ(arr[7], 7);

    ASSERT_TRUE(EMBER_CORE_ENABLE_ASSERTIONS);

    EXPECT_DEATH(arr[array_size], "");
}

TEST(array_tests, can_iterate_over_array) {
    std::size_t constexpr array_size{ 24 };

    array<std::uint32_t> arr(array_size);

    for(std::size_t i{ 0 }; i < array_size; ++i) {
        arr[i] = i;
    }

    std::size_t index{ 0 };

    for(std::uint32_t &val : arr) {
        EXPECT_EQ(val, index++);

        val *= 2;
    }
    index = 0;

    for(auto iter{ arr.begin() }; iter != arr.end(); ++iter) {
        EXPECT_EQ(*iter, index++ * 2);
    }
}

TEST(array_tests, can_initialiser_list_initialise) {
    array<std::uint32_t> arr{ 0, 1, 2, 3, 4, 5 };

    EXPECT_EQ(arr.size(), 6);
    EXPECT_EQ(arr.capacity(), 6);

    for(std::size_t i{ 0 }; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], i);
    }
}

TEST(array_tests, can_iterator_initialise) {
    std::vector<std::uint32_t> a{ 1, 2, 3, 4, 5 };
    array<std::uint32_t> b{ a.begin(), a.end() };

    ASSERT_EQ(a.size(), b.size());
    EXPECT_EQ(b.size(), b.capacity());
    for(std::size_t i{ 0 }; i < a.size(); ++i) {
        EXPECT_EQ(a[i], b[i]);
    }
}

TEST(array_tests, can_insert_new_items) {
    struct helper_type {
        int a{ 0 };
        int b{ 0 };
    };

    array<helper_type> arr{};

    ASSERT_EQ(arr.size(), 0);

    arr.push_back(helper_type{});

    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr.capacity(), 1);
    EXPECT_EQ(arr[0].a, 0);
    EXPECT_EQ(arr[0].b, 0);

    arr.emplace_back();

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.capacity(), 2);
    EXPECT_EQ(arr[1].a, 0);
    EXPECT_EQ(arr[1].b, 0);

    arr.emplace_back(1, 2);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 4);
    EXPECT_EQ(arr[2].a, 1);
    EXPECT_EQ(arr[2].b, 2);
}

TEST(array_tests, calls_destructor) {
    struct helper_type {
        std::uint32_t &count;
        helper_type(std::uint32_t &count)
            : count{ count } {};
        ~helper_type() {
            ++count;
        };
    };

    std::uint32_t count{ 0 };
    {
        array<helper_type> arr{};

        arr.emplace_back(count);
        arr.emplace_back(count);
        arr.emplace_back(count);
        arr.emplace_back(count);

        ASSERT_EQ(arr.size(), 4);
        ASSERT_EQ(count, 0);
    }

    EXPECT_EQ(count, 4);
}

TEST(array_tests, can_copy) {
    struct helper_type {
        std::uint32_t &count;
        helper_type(std::uint32_t &count)
            : count{ count } {};
        ~helper_type() {
            ++count;
        };
    };

    array<helper_type> arr_1{};
    array<helper_type> arr_2{};

    std::uint32_t count{ 0 };

    arr_1.emplace_back(count);
    arr_1.emplace_back(count);
    arr_1.emplace_back(count);

    arr_2.emplace_back(count);
    arr_2.emplace_back(count);

    arr_2 = arr_1;

    EXPECT_EQ(count, 2);
    EXPECT_EQ(arr_2.size(), arr_1.size());
    EXPECT_EQ(arr_2.capacity(), arr_1.capacity());
}

TEST(array_tests, can_move) {
    struct helper_type {
        std::uint32_t &count;
        helper_type(std::uint32_t &count)
            : count{ count } {};
        ~helper_type() {
            ++count;
        };
    };

    array<helper_type> arr_1{};
    array<helper_type> arr_2{};

    std::uint32_t count{ 0 };

    arr_1.emplace_back(count);
    arr_1.emplace_back(count);
    arr_1.emplace_back(count);

    arr_2.emplace_back(count);
    arr_2.emplace_back(count);

    std::size_t const size{ arr_1.size() };
    std::size_t const capacity{ arr_1.capacity() };

    arr_2 = std::move(arr_1);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(arr_2.size(), size);
    EXPECT_EQ(arr_2.capacity(), capacity);
    EXPECT_EQ(arr_1.data(), nullptr);
}

struct resize_destruct_helper {
    inline static std::uint32_t x{ 0 };

    ~resize_destruct_helper() {
        ++x;
    }
};

TEST(array_tests, can_resize) {
    struct construct_helper {
        std::uint32_t x{ 0 };

        construct_helper(std::uint32_t x)
            : x{ x } {
        }
    };

    array<std::uint32_t> arr_1{};
    array<construct_helper> arr_2{};
    array<resize_destruct_helper> arr_3{};

    ASSERT_EQ(arr_1.size(), 0);
    ASSERT_EQ(arr_1.capacity(), 0);
    ASSERT_EQ(arr_2.size(), 0);
    ASSERT_EQ(arr_2.capacity(), 0);
    ASSERT_EQ(arr_3.size(), 0);
    ASSERT_EQ(arr_3.capacity(), 0);

    arr_1.resize(8);
    arr_2.resize(8, 100u);

    EXPECT_EQ(arr_1.size(), 8);
    EXPECT_EQ(arr_1.capacity(), 8);
    EXPECT_EQ(arr_2.size(), 8);
    EXPECT_EQ(arr_2.capacity(), 8);

    for(auto h : arr_2) {
        EXPECT_EQ(h.x, 100);
    }

    resize_destruct_helper::x = 0;

    arr_3.emplace_back();
    arr_3.emplace_back();
    arr_3.emplace_back();
    arr_3.emplace_back();
    arr_3.emplace_back();
    arr_3.emplace_back();

    ASSERT_EQ(arr_3.size(), 6);
    ASSERT_NE(arr_3.capacity(), 0);

    arr_3.resize(2);

    EXPECT_EQ(arr_3.size(), 2);
    EXPECT_EQ(resize_destruct_helper::x, 4);
}

TEST(array_tests, resizing_calls_move_or_copy) {
    bool copied{ false };
    bool moved{ false };

    struct copy_struct {
        bool &copied;

        copy_struct(bool &copied)
            : copied{ copied } {}
        copy_struct(copy_struct const &other)
            : copied{ other.copied } {
            copied = true;
        }

        copy_struct(copy_struct &&other) = delete;
    };
    struct move_struct {
        bool &moved;

        move_struct(bool &moved)
            : moved{ moved } {}
        move_struct(move_struct &&other) noexcept
            : moved{ other.moved } {
            moved = true;
        }

        move_struct(move_struct const &other) = delete;
    };

    array<copy_struct> copy_array{};
    array<move_struct> move_array{};

    ASSERT_FALSE(copied);
    ASSERT_FALSE(moved);

    copy_array.emplace_back(copied);
    copy_array.emplace_back(copied);

    move_array.emplace_back(moved);
    move_array.emplace_back(moved);

    EXPECT_TRUE(copied);
    EXPECT_TRUE(moved);
}

TEST(array_tests, can_reserve) {
    array<std::uint32_t> arr{};

    ASSERT_EQ(arr.capacity(), 0);

    arr.reserve(5);

    EXPECT_EQ(arr.capacity(), 5);

    arr.reserve(1);

    EXPECT_EQ(arr.capacity(), 5);

    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.reserve(8);

    EXPECT_EQ(arr.capacity(), 8);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(array_tests, can_clear) {
    struct destructor_helper {
        std::uint32_t &count;
        destructor_helper(std::uint32_t &count)
            : count{ count } {};
        ~destructor_helper() {
            ++count;
        };
    };

    array<destructor_helper> arr{};

    std::uint32_t count{ 0 };

    arr.emplace_back(count);
    arr.emplace_back(count);
    arr.emplace_back(count);

    ASSERT_EQ(arr.size(), 3);

    arr.clear();

    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(count, 3);
}

// TEST(array_tests, can_shrink_to_fit) {
//     EXPECT_TRUE(false);
// }