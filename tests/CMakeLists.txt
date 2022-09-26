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

macro( add_recursive dir retVal)
    # Store unfiltered list of files into temporary list
    file( GLOB_RECURSE _tmp_list ${dir}/*.h ${dir}/*.cpp ${dir}/*.c ${dir}/*.inl )
    # Resulted list is initially empty. Copy all needed elements into it.
    set(${retVal})
    foreach(f ${_tmp_list})
        if(NOT f MATCHES "main.")
            list(APPEND ${retVal} ${f})
        endif()
    endforeach()
endmacro()

add_recursive("../server/src/" srcs)
message("RES: "  ${srcs})

add_executable(server_tests "./cmake_tests/server_tests.cpp" ${srcs})
target_include_directories(server_tests PRIVATE "../external/")
target_include_directories(server_tests PRIVATE "../server/include/")
target_link_libraries(server_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(server_tests)