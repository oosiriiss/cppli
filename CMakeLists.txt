cmake_minimum_required(VERSION 3.20)

option(ENABLE_DEBUG_UTILS "Enable debug header ./vendor/debug_utils/debug_utils.hpp" OFF)
option(CMAKE_EXPORT_COMPILE_COMMANDS "output compile_commands.json" ON)

project(cppli LANGUAGES CXX)


add_library(
     cppli-lib STATIC

     ./src/argument_container.hpp
     ./src/option.hpp
     ./src/parse.hpp
     ./src/parse_internal.hpp
)


include_directories(./vendor/debug_utils)
if(ENABLE_DEBUG_UTILS)
   target_compile_definitions(cppli-lib PUBLIC ENABLE_DEBUG_UTILS)
endif()


target_include_directories(
   cppli-lib
   PUBLIC
   "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

target_compile_features(cppli-lib PRIVATE cxx_std_23)
set_target_properties(cppli-lib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
add_subdirectory(example)
