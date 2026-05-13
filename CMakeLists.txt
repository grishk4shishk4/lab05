cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(all)

if(ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -g -coverage")
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} " -g -coverage")
endif()

endif()

add_subdirectory(banking)

option(BUILD_TESTS "Build tests" ON)

if(NOT DEFINED TESTS_NAME)
  set(TESTS_NAME check)
endif()

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(${TESTS_NAME} ${${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(${TESTS_NAME} banking gtest_main gmock_main)
  add_test(NAME ${TESTS_NAME} COMMAND ${TESTS_NAME})
endif()

message("flags:" ${CMAKE_CXX_FLAGS})
message("flags C:" ${CMAKE_C_FLAGS})
message("BUILD_TESTS:" ${BUILD_TESTS})
message("ENABLE_COVERAGE:" ${ENABLE_COVERAGE})
