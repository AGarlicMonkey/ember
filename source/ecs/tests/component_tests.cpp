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

    entity entity{ manager.create() };

    auto const &bool_comp{ manager.add_component<bool_component>(entity, true) };
    EXPECT_EQ(bool_comp.value, true);
}

//Multiple

//Check

//Get

//Remove

