cmake_minimum_required(VERSION 3.21)

project(cppli-examples LANGUAGES CXX) 

function(Add_example name) 
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} PRIVATE cppli-lib)
   target_compile_features(${name} PRIVATE cxx_std_23)
   message("Registered ${name}")
endfunction()

Add_example(example1)
