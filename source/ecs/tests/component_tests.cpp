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

TEST(component_tests, can_add_a_single_component) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };

    auto const &bool_comp{ manager.add_component<bool_component>(entity_1, true) };
    EXPECT_EQ(bool_comp.value, true);

    auto const &float_comp{ manager.add_component<float_component>(entity_2) };
    EXPECT_EQ(float_comp.value, float_component{}.value);
}

TEST(component_tests, can_check_for_a_component) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };

    manager.add_component<bool_component>(entity_1);
    manager.add_component<bool_component>(entity_2);
    manager.add_component<float_component>(entity_3);

    EXPECT_TRUE(manager.has_component<bool_component>(entity_1));
    EXPECT_FALSE(manager.has_component<float_component>(entity_1));

    EXPECT_TRUE(manager.has_component<bool_component>(entity_2));
    EXPECT_FALSE(manager.has_component<float_component>(entity_2));

    EXPECT_TRUE(manager.has_component<float_component>(entity_3));
    EXPECT_FALSE(manager.has_component<bool_component>(entity_3));
}

TEST(component_tests, can_get_a_component) {
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };
    entity entity_3{ manager.create() };
    entity entity_4{ manager.create() };

    manager.add_component<bool_component>(entity_1, true);
    manager.add_component<bool_component>(entity_2, false);
    manager.add_component<int_component>(entity_3, 100);
    manager.add_component<int_component>(entity_4, 200);

    EXPECT_EQ(manager.get_component<bool_component>(entity_1).value, true);
    EXPECT_EQ(manager.get_component<bool_component>(entity_2).value, false);
    EXPECT_EQ(manager.get_component<int_component>(entity_3).value, 100);
    EXPECT_EQ(manager.get_component<int_component>(entity_4).value, 200);
}

TEST(component_tests, can_remove_a_component) {
    entity_manager manager{};

    entity entity_1{ manager.create() };

    manager.add_component<bool_component>(entity_1);

    ASSERT_TRUE(manager.has_component<bool_component>(entity_1));

    manager.remove_component<bool_component>(entity_1);

    EXPECT_FALSE(manager.has_component<bool_component>(entity_1));
}

TEST(component_tests, can_add_multiple_components){
    entity_manager manager{};

    entity entity_1{ manager.create() };
    entity entity_2{ manager.create() };

    manager.add_component<bool_component>(entity_1, true);
    manager.add_component<float_component>(entity_1);
    manager.add_component<complex_component>(entity_1);

    manager.add_component<float_component>(entity_2);
    manager.add_component<complex_component>(entity_2);

    EXPECT_TRUE(manager.has_component<bool_component>(entity_1));
    EXPECT_TRUE(manager.has_component<float_component>(entity_1));
    EXPECT_TRUE(manager.has_component<complex_component>(entity_1));

    EXPECT_EQ(manager.get_component<bool_component>(entity_1).value, true);
    EXPECT_EQ(manager.add_component<bool_component>(entity_1, false).value, false);

    EXPECT_TRUE(manager.has_component<float_component>(entity_2));
    EXPECT_TRUE(manager.has_component<complex_component>(entity_2));
}

TEST(component_tests, can_remove_from_multiple_components) {
    entity_manager manager{};

    entity entity_1{ manager.create() };

    manager.add_component<bool_component>(entity_1);
    manager.add_component<float_component>(entity_1);
    manager.add_component<complex_component>(entity_1);

    ASSERT_TRUE(manager.has_component<bool_component>(entity_1));
    ASSERT_TRUE(manager.has_component<float_component>(entity_1));
    ASSERT_TRUE(manager.has_component<complex_component>(entity_1));

    manager.remove_component<float_component>(entity_1);

    EXPECT_TRUE(manager.has_component<bool_component>(entity_1));
    EXPECT_FALSE(manager.has_component<float_component>(entity_1));
    EXPECT_TRUE(manager.has_component<complex_component>(entity_1));

    manager.remove_component<complex_component>(entity_1);

    EXPECT_TRUE(manager.has_component<bool_component>(entity_1));
    EXPECT_FALSE(manager.has_component<float_component>(entity_1));
    EXPECT_FALSE(manager.has_component<complex_component>(entity_1));
}