cmake_minimum_required(VERSION 3.14)

# What about this option when library is public?

include(./cmake/log.cmake)

project(cppli 
   LANGUAGES CXX
   VERSION 0.1.0
)
log("CPPLI version: " ${PROJECT_VERSION})
log("Project binary dir:" ${PROJECT_BINARY_DIR})

include(./cmake/project_is_top_level.cmake)

option(CPPLI_ENABLE_DEBUG_UTILS "Enable debug utilities for cppli" ${PROJECT_IS_TOP_LEVEL})
option(CPPLI_BUILD_TESTS "Whether tests should be compiled for cppli" ${PROJECT_IS_TOP_LEVEL})
option(CPPLI_BUILD_EXAMPLES "Whether examples should be compiled for cppli" ${PROJECT_IS_TOP_LEVEL})
option(CPPLI_INSTALL "Whether library will be installed" ${PROJECT_IS_TOP_LEVEL})

if(PROJECT_IS_TOP_LEVEL)
   option(CMAKE_EXPORT_COMPILE_COMMANDS "output compile_commands.json" ON)
endif()


add_library(
     cppli-lib INTERFACE
     ./include/cppli/argument_container.hpp
     ./include/cppli/option.hpp
     ./include/cppli/cppli.hpp
     ./include/cppli/parse_internal.hpp
)

set_target_properties(
   cppli-lib PROPERTIES
   CXX_VISIBILITY_PRESET hidden
   VISIBILITY_INLINES_HIDDEN YES
   VERSION "${PROJECT_VERSION}"
   SOVERSION "${PROJECT_VERSION_MAJOR}"
   EXPORT_NAME cppli
   OUTPUT_NAME cppli
)

add_library(cppli::cppli ALIAS cppli-lib)

if(CPPLI_ENABLE_DEBUG_UTILS)
   target_compile_definitions(cppli-lib INTERFACE CPPLI_ENABLE_DEBUG_UTILS)
endif()


# g++ requires this library for std::stacktrace
if(CPPLI_ENABLE_DEBUG_UTILS AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")) 
   target_link_libraries(cppli-lib INTERFACE stdc++exp)
endif()


target_include_directories(
   cppli-lib
   INTERFACE
   "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(cppli-lib INTERFACE cxx_std_23)
set_target_properties(cppli-lib PROPERTIES LINKER_LANGUAGE CXX)

if(CPPLI_INSTALL) 
   include(./cmake/install.cmake)
endif()


if(CPPLI_BUILD_TESTS)
   add_subdirectory(test)
endif()

if(CPPLI_BUILD_EXAMPLES)
   add_subdirectory(example)
endif()
