cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    cppli
    VERSION 0.1.0
    DESCRIPTION "C++ CLI Library"
    LANGUAGES CXX
)

include(FetchContent)
FetchContent_Declare(
   logzy
   GIT_REPOSITORY https://github.com/oosiriiss/logzy
   GIT_TAG 0.1
)
FetchContent_MakeAvailable(logzy)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    cppli_cppli
    src/argparser.cpp 
    src/cppli.cpp 
    src/option.cpp
)
add_library(cppli::cppli ALIAS cppli_cppli)

include(GenerateExportHeader)
generate_export_header(
    cppli_cppli
    BASE_NAME cppli
    EXPORT_FILE_NAME export/cppli/cppli_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(cppli_cppli PUBLIC CPPLI_STATIC_DEFINE)
endif()

set_target_properties(
    cppli_cppli PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME cppli
    OUTPUT_NAME cppli
)

target_include_directories(
    cppli_cppli ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    cppli_cppli SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_link_libraries(
   cppli_cppli
   PUBLIC
      logzy::logzy
)

target_compile_features(cppli_cppli PUBLIC cxx_std_23)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${cppli_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT cppli_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of cppli"
  )
endif()

include(cmake/dev-mode.cmake)
