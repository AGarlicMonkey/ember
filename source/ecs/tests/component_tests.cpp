#include <ember/ecs/entity_manager.hpp>
#include <gtest/gtest.h>

using namespace ember::ecs;

struct bool_component {
    bool value{ false };
};

struct float_component {
    float value{ 0.0f };
};

struct complex_component{
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

    manager.add_component<bool_component>(entity_1);
    manager.add_component<float_component>(entity_2);

    EXPECT_TRUE(manager.has_component<bool_component>(entity_1));
    EXPECT_FALSE(manager.has_component<float_component>(entity_1));

    EXPECT_TRUE(manager.has_component<float_component>(entity_2));
    EXPECT_FALSE(manager.has_component<bool_component>(entity_2));
}

//Get

//Remove

//Multiple