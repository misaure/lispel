cmake_minimum_required(VERSION 2.6)

project(lispeltests)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ..)

add_definitions(-fpermissive -Wextra)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests lexertests.cpp gtests.cpp)
target_link_libraries(runTests ispel ${GTEST_LIBRARIES} pthread)
