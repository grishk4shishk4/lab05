cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(all)

add_subdirectory(banking)

if(NOT DEFINED TESTS_NAME)
	set(TESTS_NAME check)
endif()

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(third-party/gtest)
	file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
	add_executable(${TESTS_NAME} ${${PROJECT_NAME_TEST_SOURCES})
	target_link_libraries(${TESTS_NAME} banking gtest_main gmock_main)
	add_test(NAME ${TESTS_NAME} COMMAND ${TESTS_NAME})
endif()

