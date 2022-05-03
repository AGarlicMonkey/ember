#include <ember/ecs/entity.hpp>
#include <ember/ecs/entity_manager.hpp>
#include <gtest/gtest.h>

using namespace ember::ecs;

TEST(entity_tests, entity_default_inits_to_null_entity) {
    entity entity{};

    EXPECT_EQ(entity, null_entity);
}

TEST(entity_tests, can_create_an_entity) {
    entity_manager manager{};
    entity entity{ manager.create() };

    EXPECT_NE(entity, null_entity);
    EXPECT_TRUE(manager.is_valid(entity));
}

TEST(entity_tests, can_destroy_an_entity){
    entity_manager manager{};
    entity entity{ manager.create() };

    ASSERT_NE(entity, null_entity);
    ASSERT_TRUE(manager.is_valid(entity));

    manager.destroy(entity);

    EXPECT_EQ(entity, null_entity);
    EXPECT_FALSE(manager.is_valid(entity));
}

TEST(entity_tests, can_destroy_all_entities) {
    entity_manager manager{};
    entity a{ manager.create() };
    entity b{ manager.create() };
    entity c{ manager.create() };
    entity d{ manager.create() };
    entity e{ manager.create() };

    ASSERT_TRUE(manager.is_valid(a));
    ASSERT_TRUE(manager.is_valid(b));
    ASSERT_TRUE(manager.is_valid(c));
    ASSERT_TRUE(manager.is_valid(d));
    ASSERT_TRUE(manager.is_valid(e));

    manager.destroy_all();

    EXPECT_FALSE(manager.is_valid(a));
    EXPECT_FALSE(manager.is_valid(b));
    EXPECT_FALSE(manager.is_valid(c));
    EXPECT_FALSE(manager.is_valid(d));
    EXPECT_FALSE(manager.is_valid(e));
}

TEST(entity_tests, entities_are_unique_between_managers) {
    entity_manager manager_a{};
    entity_manager manager_b{};

    entity a{ manager_a.create() };
    entity b{ manager_b.create() };

    EXPECT_NE(a, b);
    EXPECT_FALSE(manager_a.is_valid(b));
    EXPECT_FALSE(manager_b.is_valid(a));
}