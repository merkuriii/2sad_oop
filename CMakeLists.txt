cmake_minimum_required(VERSION 3.10)
project(lab2_oop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(${CMAKE_PROJECT_NAME}_lib src/money.cpp)
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

# Добавление тестов
enable_testing()
add_executable(tests tests/test2.cpp)
target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest_main)
target_include_directories(tests PRIVATE include)

# Добавление тестов в тестовый набор
add_test(NAME MyProjectTests COMMAND $<TARGET_FILE:tests>)