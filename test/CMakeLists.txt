cmake_minimum_required(VERSION 3.14)

project(cppliTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(cppli REQUIRED)
  enable_testing()
endif()

include(FetchContent)
FetchContent_Declare(
   tasty
   GIT_REPOSITORY https://github.com/oosiriiss/tasty
   GIT_TAG 0.2.9
)
FetchContent_MakeAvailable(tasty)


add_library(cppliTestCommon INTERFACE)
target_include_directories(cppliTestCommon INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(cppliTestCommon INTERFACE cppli::cppli)
target_link_libraries(cppliTestCommon INTERFACE tasty::tasty)

function(Register_Test name)
   add_executable(${name} src/${name}.cpp)
   target_link_libraries(${name} PRIVATE cppliTestCommon)
   target_compile_features(${name} PRIVATE cxx_std_23)
   add_test(NAME ${name} COMMAND ${name})
endfunction()
# ---- Tests ----

Register_Test(argparser_test)

# ---- End-of-file commands ----

add_folders(Test)
