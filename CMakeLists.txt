project(generators)

cmake_minimum_required (VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)

# We have gtest/gmock as a Git submodule, add subdirectory to pick
# this up we we can use it as a dependency
add_subdirectory(lib/googletest)

add_executable(main src/main.cpp)
#target_link_libraries(main)
target_include_directories (main PRIVATE ${PROJECT_SOURCE_DIR}/include)

file (GLOB testSources test/*.cpp)
add_executable (gen_test ${testSources})
target_link_libraries (gen_test gmock gmock_main gtest pthread)
target_include_directories (gen_test PRIVATE ${gtest_SOURCE_DIR}/include)
target_include_directories (gen_test PRIVATE ${gmock_SOURCE_DIR}/include)
target_include_directories (gen_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

# This enables the "make test" target, although for TDD it's likely
# you'll want to use the "make unit" target.
enable_testing ()

# Add the unit tests executable 
add_test (Unit gen_test)

# And a custom target as a convenience for running "make unit" as a
# specific target to support the TDD cycle
add_custom_target(unit
  COMMAND ${CMAKE_BINARY_DIR}/gen_test --gtest_color=true
  )
