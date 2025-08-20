cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CLI)
set(SOURCES 
src/main.cpp 
src/cli.cpp 	
src/argparser.cpp 
src/option.cpp
)

add_executable(CLI ${SOURCES})
