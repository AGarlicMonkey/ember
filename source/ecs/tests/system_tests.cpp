#include <ember/ecs/entity_manager.hpp>
#include <gtest/gtest.h>

#include <vector>

using namespace ember;

struct bool_component {
    bool value{ false };
};

struct float_component {
    float value{ 0.0f };
};

struct int_component {
    std::int32_t value{ 0 };
};

struct complex_component {
    std::int32_t a{ 0 };
    std::int32_t b{ 0 };
    std::int32_t c{ 0 };
};

TEST(system_tests, can_use_lambda_function) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };
    entity entity_4{ manager.create() };

    manager.add_component<bool_component>(entity_1);

    manager.add_component<bool_component>(entity_2);
    manager.add_component<int_component>(entity_2);

    manager.add_component<bool_component>(entity_3);
    manager.add_component<int_component>(entity_3);
    manager.add_component<float_component>(entity_3);
    manager.add_component<complex_component>(entity_3);

    manager.add_component<float_component>(entity_4);
    manager.add_component<complex_component>(entity_4);

    ASSERT_EQ(manager.get_component<bool_component>(entity_1).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_2).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_3).value, false);

    ASSERT_EQ(manager.get_component<int_component>(entity_2).value, 0);
    ASSERT_EQ(manager.get_component<int_component>(entity_3).value, 0);

    ASSERT_EQ(manager.get_component<complex_component>(entity_3).a, 0);
    ASSERT_EQ(manager.get_component<complex_component>(entity_3).b, 0);
    ASSERT_EQ(manager.get_component<complex_component>(entity_3).c, 0);
    ASSERT_EQ(manager.get_component<complex_component>(entity_4).a, 0);
    ASSERT_EQ(manager.get_component<complex_component>(entity_4).b, 0);
    ASSERT_EQ(manager.get_component<complex_component>(entity_4).c, 0);

    ASSERT_EQ(manager.get_component<float_component>(entity_3).value, 0);
    ASSERT_EQ(manager.get_component<float_component>(entity_4).value, 0);

    std::int32_t bool_count{ 0 };
    manager.for_each([&bool_count](bool_component &comp) {
        comp.value = true;

        ++bool_count;
    });

    std::int32_t int_count{ 0 };
    manager.for_each([&int_count](int_component &comp) {
        comp.value = 1234;

        ++int_count;
    });

    std::int32_t multiple_count{ 0 };
    manager.for_each([&multiple_count](float_component &f_comp, complex_component &c_comp) {
        f_comp.value = 1.0f;
        c_comp.a     = 1;
        c_comp.b     = 2;
        c_comp.c     = 3;

        ++multiple_count;
    });

    EXPECT_EQ(manager.get_component<bool_component>(entity_1).value, true);

    EXPECT_EQ(manager.get_component<bool_component>(entity_2).value, true);
    EXPECT_EQ(manager.get_component<int_component>(entity_2).value, 1234);

    EXPECT_EQ(manager.get_component<bool_component>(entity_3).value, true);
    EXPECT_EQ(manager.get_component<int_component>(entity_3).value, 1234);
    EXPECT_EQ(manager.get_component<complex_component>(entity_3).a, 1);
    EXPECT_EQ(manager.get_component<complex_component>(entity_3).b, 2);
    EXPECT_EQ(manager.get_component<complex_component>(entity_3).c, 3);
    EXPECT_EQ(manager.get_component<float_component>(entity_3).value, 1.0f);

    EXPECT_EQ(manager.get_component<complex_component>(entity_4).a, 1);
    EXPECT_EQ(manager.get_component<complex_component>(entity_4).b, 2);
    EXPECT_EQ(manager.get_component<complex_component>(entity_4).c, 3);
    EXPECT_EQ(manager.get_component<float_component>(entity_4).value, 1.0f);

    EXPECT_EQ(bool_count, 3);
    EXPECT_EQ(int_count, 2);
    EXPECT_EQ(multiple_count, 2);
}

void bool_setter(bool_component &bool_comp) {
    bool_comp.value = true;
}

TEST(system_tests, can_use_free_function) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };
    entity entity_4{ manager.create() };
    entity entity_5{ manager.create() };
    entity entity_6{ manager.create() };

    manager.add_component<bool_component>(entity_1);

    manager.add_component<bool_component>(entity_2);
    manager.add_component<complex_component>(entity_2);

    manager.add_component<bool_component>(entity_3);
    manager.add_component<float_component>(entity_3);
    manager.add_component<int_component>(entity_3);

    manager.add_component<bool_component>(entity_4);
    manager.add_component<bool_component>(entity_5);
    manager.add_component<bool_component>(entity_6);

    ASSERT_EQ(manager.get_component<bool_component>(entity_1).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_2).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_3).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_4).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_5).value, false);
    ASSERT_EQ(manager.get_component<bool_component>(entity_6).value, false);

    manager.for_each(bool_setter);

    EXPECT_EQ(manager.get_component<bool_component>(entity_1).value, true);
    EXPECT_EQ(manager.get_component<bool_component>(entity_2).value, true);
    EXPECT_EQ(manager.get_component<bool_component>(entity_3).value, true);
    EXPECT_EQ(manager.get_component<bool_component>(entity_4).value, true);
    EXPECT_EQ(manager.get_component<bool_component>(entity_5).value, true);
    EXPECT_EQ(manager.get_component<bool_component>(entity_6).value, true);
}

TEST(system_tests, can_use_member_function) {
    struct member_function_helper {
        std::int32_t count{ 0 };
        void int_setter(int_component &int_comp) {
            int_comp.value = 100;
            ++count;
        }
    };

    entity_manager manager{};
    member_function_helper helper{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };
    entity entity_4{ manager.create() };
    entity entity_5{ manager.create() };
    entity entity_6{ manager.create() };

    manager.add_component<int_component>(entity_1);

    manager.add_component<bool_component>(entity_2);
    manager.add_component<complex_component>(entity_2);

    manager.add_component<bool_component>(entity_3);
    manager.add_component<float_component>(entity_3);
    manager.add_component<int_component>(entity_3);

    manager.add_component<int_component>(entity_4);
    manager.add_component<int_component>(entity_5);
    manager.add_component<int_component>(entity_6);

    ASSERT_EQ(manager.get_component<int_component>(entity_1).value, 0);
    ASSERT_EQ(manager.get_component<int_component>(entity_3).value, 0);
    ASSERT_EQ(manager.get_component<int_component>(entity_4).value, 0);
    ASSERT_EQ(manager.get_component<int_component>(entity_5).value, 0);
    ASSERT_EQ(manager.get_component<int_component>(entity_6).value, 0);

    manager.for_each(&member_function_helper::int_setter, &helper);

    EXPECT_EQ(manager.get_component<int_component>(entity_1).value, 100);
    EXPECT_EQ(manager.get_component<int_component>(entity_3).value, 100);
    EXPECT_EQ(manager.get_component<int_component>(entity_4).value, 100);
    EXPECT_EQ(manager.get_component<int_component>(entity_5).value, 100);
    EXPECT_EQ(manager.get_component<int_component>(entity_6).value, 100);
    EXPECT_EQ(helper.count, 5);
}

TEST(system_tests, entities_components_are_removed) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };

    manager.add_component<int_component>(entity_1);
    manager.add_component<bool_component>(entity_1);
    manager.add_component<complex_component>(entity_1);

    manager.add_component<int_component>(entity_2);
    manager.add_component<bool_component>(entity_2);
    manager.add_component<complex_component>(entity_2);

    manager.add_component<int_component>(entity_3);
    manager.add_component<bool_component>(entity_3);
    manager.add_component<complex_component>(entity_3);

    std::int32_t int_comp_count{ 0 };
    std::int32_t bool_comp_count{ 0 };
    std::int32_t complex_comp_count{ 0 };

    manager.for_each([&](int_component const &comp) {
        ++int_comp_count;
    });
    manager.for_each([&](bool_component const &comp) {
        ++bool_comp_count;
    });
    manager.for_each([&](complex_component const &comp) {
        ++complex_comp_count;
    });

    ASSERT_EQ(int_comp_count, 3);
    ASSERT_EQ(bool_comp_count, 3);
    ASSERT_EQ(complex_comp_count, 3);

    int_comp_count     = 0;
    bool_comp_count    = 0;
    complex_comp_count = 0;

    manager.remove_component<int_component>(entity_2);

    manager.for_each([&](int_component const &comp) {
        ++int_comp_count;
    });
    manager.for_each([&](bool_component const &comp) {
        ++bool_comp_count;
    });
    manager.for_each([&](complex_component const &comp) {
        ++complex_comp_count;
    });

    EXPECT_EQ(int_comp_count, 2);
    EXPECT_EQ(bool_comp_count, 3);
    EXPECT_EQ(complex_comp_count, 3);

    int_comp_count     = 0;
    bool_comp_count    = 0;
    complex_comp_count = 0;

    manager.destroy(entity_1);

    manager.for_each([&](int_component const &comp) {
        ++int_comp_count;
    });
    manager.for_each([&](bool_component const &comp) {
        ++bool_comp_count;
    });
    manager.for_each([&](complex_component const &comp) {
        ++complex_comp_count;
    });

    EXPECT_EQ(int_comp_count, 1);
    EXPECT_EQ(bool_comp_count, 2);
    EXPECT_EQ(complex_comp_count, 2);

    int_comp_count     = 0;
    bool_comp_count    = 0;
    complex_comp_count = 0;

    manager.destroy_all();

    manager.for_each([&](int_component const &comp) {
        ++int_comp_count;
    });
    manager.for_each([&](bool_component const &comp) {
        ++bool_comp_count;
    });
    manager.for_each([&](complex_component const &comp) {
        ++complex_comp_count;
    });

    EXPECT_EQ(int_comp_count, 0);
    EXPECT_EQ(bool_comp_count, 0);
    EXPECT_EQ(complex_comp_count, 0);
}

TEST(system_tests, can_get_entity) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };

    manager.add_component<int_component>(entity_1);
    manager.add_component<int_component>(entity_2);
    manager.add_component<int_component>(entity_3);

    manager.add_component<bool_component>(entity_2);
    manager.add_component<complex_component>(entity_3);

    std::vector<entity> found_list{};

    manager.for_each([&](entity entity, int_component &comp) {
        found_list.push_back(entity);
    });

    EXPECT_TRUE(std::find(found_list.begin(), found_list.end(), entity_1) != found_list.end());
    EXPECT_TRUE(std::find(found_list.begin(), found_list.end(), entity_2) != found_list.end());
    EXPECT_TRUE(std::find(found_list.begin(), found_list.end(), entity_3) != found_list.end());
}

/*
TEST(system_tests, can_exclude_components_with_lambda_function) {
}
*/

/*
TEST(system_tests, can_exclude_components_with_free_function) {
}
*/

/* 
TEST(system_tests, can_exclude_components_with_member_function) {
} 
*/