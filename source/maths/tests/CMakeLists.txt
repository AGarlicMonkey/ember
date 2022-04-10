find_package(GTest REQUIRED CONFIG)

#Vector
add_executable(vector_test vector_tests.cpp)
target_link_libraries(vector_test PRIVATE GTest::gtest_main ember_maths)
add_test(NAME vector_test COMMAND vector_test)

#Matrix
add_executable(matrix_test matrix_tests.cpp)
target_link_libraries(matrix_test PRIVATE GTest::gtest_main ember_maths)
add_test(NAME matrix_test COMMAND matrix_test)

#Quaternion
add_executable(quaternion_test quaternion_tests.cpp)
target_link_libraries(quaternion_test PRIVATE GTest::gtest_main ember_maths)
add_test(NAME quaternion_test COMMAND quaternion_test)