cmake_minimum_required(VERSION 3.14)

project(cppli_tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
   tasty
   GIT_REPOSITORY https://github.com/oosiriiss/tasty
   GIT_TAG 0.2.9.2
)
FetchContent_MakeAvailable(tasty)

function(Register_Test name)
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} PRIVATE cppli-lib tasty::tasty)
   target_compile_features(${name} PRIVATE cxx_std_23)
   add_test(NAME ${name} COMMAND ${name})
   message("Test ${name} registered")
endfunction()


Register_Test(arg_container_test)
Register_Test(define_arguments_test)
Register_Test(parse_test)
Register_Test(general_api_test)
