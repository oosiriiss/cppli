cmake_minimum_required(VERSION 3.14)

# What about this option when library is public?


project(cppli LANGUAGES CXX)
include(./cmake/project_is_top_level.cmake)

option(ENABLE_DEBUG_UTILS "Enable debug header ./vendor/debug_utils/debug_utils.hpp" ${PROJECT_IS_TOP_LEVEL})
option(CPPLI_BUILD_TESTS "Whether tests should be compiled" ${PROJECT_IS_TOP_LEVEL})
option(CPPLI_BUILD_EXAMPLES "Whether examples should be compiled" ${PROJECT_IS_TOP_LEVEL})
if(PROJECT_IS_TOP_LEVEL)
   option(CMAKE_EXPORT_COMPILE_COMMANDS "output compile_commands.json" ON)
endif()


add_library(
     cppli-lib STATIC

     ./src/argument_container.hpp
     ./src/option.hpp
     ./src/parse.hpp
     ./src/parse_internal.hpp
)

add_library(cppli::cppli ALIAS cppli-lib)

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


if(CPPLI_BUILD_TESTS)
   add_subdirectory(test)
endif()

if(CPPLI_BUILD_EXAMPLES)
   add_subdirectory(example)
endif()
