#include <ember/ecs/entity_manager.hpp>
#include <gtest/gtest.h>

using namespace ember::ecs;

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

/*
TEST(system_tests, can_use_free_function) {
}
*/

/*
TEST(system_tests, can_use_member_function) {
}
*/

/*
TEST(system_tests, can_get_entity) {
}
*/

/*
TEST(system_tests, entities_components_are_removed) {
    //Remove individual components
    //Remove whole entities
}
*/

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