cmake_minimum_required(VERSION 3.10)
project(QuickestPathTeam7)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include Google Test
add_subdirectory(googletest)

# Add Hello World executable
add_executable(hello_world code/hello_world.cpp)

# Add Unit Tests executable
add_executable(unit_tests code/unit_tests.cpp)
target_link_libraries(unit_tests gtest gtest_main)

# Enable testing
enable_testing()
add_test(NAME UnitTests COMMAND unit_tests)