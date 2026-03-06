log("Installing cppli ${PROJECT_VERSION}")

set(PACKAGE "cppli")

if(PROJECT_IS_TOP_LEVEL)
   # Pre-setting GNU include dir with versioned one
   set(CMAKE_INSTALL_INCLUDEDIR "include/${PACKAGE}-${PROJECT_VERSION}" CACHE STRING "")
   set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(./cmake/log.cmake)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

log("install include dir:" ${CMAKE_INSTALL_INCLUDEDIR})
log("install binary dir:" ${CMAKE_INSTALL_BINDIR})
log("install library dir:" ${CMAKE_INSTALL_LIBDIR})

# Installing headers
install(
   DIRECTORY
   ./include/
   DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
   COMPONENT cppli_Development
)

# Installing targets
install(
   TARGETS cppli-lib
   EXPORT cppliTargets
   # Setting runtime and library for the future,
   # as they aren't needed with current header-only version
   RUNTIME COMPONENT cppli_Runtime
   LIBRARY COMPONENT cppli_Runtime 
   NAMELINK_COMPONENT cppli_Development
   ARCHIVE COMPONENT cppli_Development
   INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

log("cppliConfigVersion.cmake wrote to location:" "${PROJECT_BINARY_DIR}/${PACKAGE}ConfigVersion.cmake")
write_basic_package_version_file(
   "${PROJECT_BINARY_DIR}/${PACKAGE}ConfigVersion.cmake"
   VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion
)

set(
   CPPLI_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PACKAGE}"
   CACHE STRING "CMake package config location. Relative to the install prefix"
)
set_property(CACHE CPPLI_INSTALL_CMAKE_DIR PROPERTY TYPE PATH)
log("CMake package config location:" ${CPPLI_INSTALL_CMAKE_DIR})

install(
   FILES ./cmake/install-config.cmake
   DESTINATION "${CPPLI_INSTALL_CMAKE_DIR}"
   RENAME "${PACKAGE}Config.cmake"
   COMPONENT cppli_Development
)

install(
   FILES "${PROJECT_BINARY_DIR}/${PACKAGE}ConfigVersion.cmake"
   DESTINATION "${CPPLI_INSTALL_CMAKE_DIR}"
   COMPONENT cppli_Development 
) 

install(
   EXPORT cppliTargets
   NAMESPACE cppli::
   DESTINATION "${CPPLI_INSTALL_CMAKE_DIR}"
   COMPONENT cppli_Development 
) 

if(PROJECT_IS_TOP_LEVEL)
   include(CPack)
endif()
