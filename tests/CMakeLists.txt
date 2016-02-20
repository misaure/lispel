cmake_minimum_required(VERSION 2.6)

project(lispeltests)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ..)
link_directories(../lispel)
add_definitions(-g -fpermissive -Wextra)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests lexertests envtests factorytests gtests)
target_link_libraries(runTests ispel ${GTEST_LIBRARIES} pthread)
