find_package(GTest REQUIRED CONFIG)

#Allocations
add_executable(allocation_test allocation_tests.cpp)
target_link_libraries(allocation_test PRIVATE GTest::gtest_main ember_memory)
add_test(NAME allocation_test COMMAND allocation_test)

#UniquePtr
add_executable(unique_ptr_test unique_ptr_tests.cpp)
target_link_libraries(unique_ptr_test PRIVATE GTest::gtest_main ember_memory)
add_test(NAME unique_ptr_test COMMAND unique_ptr_test)