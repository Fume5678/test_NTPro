cmake_minimum_required(VERSION 3.14)

project(my_tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


## Add your test files here
enable_testing()

add_executable(order_handler "./cmake_tests/order_handler.cpp")

target_link_libraries(order_handler gtest_main)

include(GoogleTest)
gtest_discover_tests(order_handler)