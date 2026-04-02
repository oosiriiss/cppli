cmake_minimum_required(VERSION 3.21)


project(cppli_tests LANGUAGES CXX)

include(../cmake/log.cmake)
include(CTest)
enable_testing()

# Building just tests
if(PROJECT_IS_TOP_LEVEL)
   find_package(cppli REQUIRED)
endif()

include(FetchContent)
FetchContent_Declare(
   tasty
   GIT_REPOSITORY https://github.com/oosiriiss/tasty
   GIT_TAG 3b7b6b71e69882dad8961057b172f7e2aaebc524 # 0.2.11
)
FetchContent_MakeAvailable(tasty)

function(Register_Test name)
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} PRIVATE cppli::cppli PRIVATE tasty::tasty)
   target_compile_features(${name} PRIVATE cxx_std_23)
   add_test(NAME ${name} COMMAND ${name})
   log("Test ${name} registered")
endfunction()


Register_Test(arg_container_test)
Register_Test(define_arguments_test)
Register_Test(parse_test)
Register_Test(general_api_test)
