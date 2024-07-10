# Measurement library target
add_library(${LIB_NAME} measurement/measurement.c)

# Measurement unit testing
add_executable(${APP_NAME_TEST} measurement/measurement_test.cpp)
target_link_libraries(${APP_NAME_TEST} GTest::gtest_main ${LIB_NAME})
include(GoogleTest)
gtest_discover_tests(${APP_NAME_TEST})
