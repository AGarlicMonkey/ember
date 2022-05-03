find_package(GTest REQUIRED CONFIG)

#Entity
add_executable(entity_test entity_tests.cpp)
target_link_libraries(entity_test PRIVATE GTest::gtest_main ember_ecs)
add_test(NAME entity_test COMMAND entity_test)