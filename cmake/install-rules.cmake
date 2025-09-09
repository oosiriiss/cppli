if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/cppli-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package cppli)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT cppli_Development
)

install(
    TARGETS cppli_cppli
    EXPORT cppliTargets
    RUNTIME #
    COMPONENT cppli_Runtime
    LIBRARY #
    COMPONENT cppli_Runtime
    NAMELINK_COMPONENT cppli_Development
    ARCHIVE #
    COMPONENT cppli_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    cppli_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE cppli_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(cppli_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${cppli_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT cppli_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${cppli_INSTALL_CMAKEDIR}"
    COMPONENT cppli_Development
)

install(
    EXPORT cppliTargets
    NAMESPACE cppli::
    DESTINATION "${cppli_INSTALL_CMAKEDIR}"
    COMPONENT cppli_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
