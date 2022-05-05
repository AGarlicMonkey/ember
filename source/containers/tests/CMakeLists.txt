find_package(GTest REQUIRED CONFIG)

#Array
add_executable(array_test array_tests.cpp)
target_link_libraries(array_test PRIVATE GTest::gtest_main ember_containers)
add_test(NAME array_test COMMAND array_test)

#Sparse Set
add_executable(sparse_set_test sparse_set_tests.cpp)
target_link_libraries(sparse_set_test PRIVATE GTest::gtest_main ember_containers)
add_test(NAME sparse_set_test COMMAND sparse_set_test)