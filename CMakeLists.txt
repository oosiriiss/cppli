cmake_minimum_required(VERSION 3.14)

project(cppli LANGUAGES CXX)


if(NOT DEFINED EXPORT_COMPILE_COMMANDS) 
   set(EXPORT_COMPILE_COMMANDS ON)
endif()

add_library(
     cppli-lib STATIC

     ./src/argument_container.hpp
     ./src/argument.hpp
)

target_include_directories(
   cppli-lib
   PUBLIC
   "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)
target_compile_features(cppli-lib PRIVATE cxx_std_23)
set_target_properties(cppli-lib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
add_subdirectory(example)
