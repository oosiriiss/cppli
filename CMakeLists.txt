cmake_minimum_required(VERSION 3.14)

project(cppli LANGUAGES CXX)


if(NOT DEFINED EXPORT_COMPILE_COMMANDS) 
   set(EXPORT_COMPILE_COMMANDS ON)
endif()

add_library(
     cppli-lib STATIC

     ./src/lib1.cpp
     ./src/lib1.hpp
)

target_include_directories(
   cppli-lib
   PUBLIC
   "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)
target_compile_features(cppli-lib PRIVATE cxx_std_23)


add_subdirectory(test)
add_subdirectory(example)
