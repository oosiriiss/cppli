cmake_minimum_required(VERSION 3.14)

project(cppliTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(cppli REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(cppli_test source/cppli_test.cpp)
target_link_libraries(cppli_test PRIVATE cppli::cppli)
target_compile_features(cppli_test PRIVATE cxx_std_23)

add_test(NAME cppli_test COMMAND cppli_test)

# ---- End-of-file commands ----

add_folders(Test)
