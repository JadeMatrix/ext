CMAKE_MINIMUM_REQUIRED( VERSION 3.1 FATAL_ERROR #[[
    Requiring 3.1+:
        WRITE_BASIC_PACKAGE_VERSION_FILE( ... )
]] )

PROJECT(
    "JadeMatrix-ext"
    VERSION 0.1
    LANGUAGES CXX
)

INCLUDE( CTest )
INCLUDE( GNUInstallDirs )

IF( EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
    INCLUDE( "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
    CONAN_SET_FIND_PATHS()
ENDIF()


ADD_LIBRARY( ext INTERFACE )
TARGET_INCLUDE_DIRECTORIES(
    ext
    INTERFACE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


INSTALL(
    TARGETS ext
    EXPORT "jadematrix-ext-config"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT core
)
INSTALL(
    DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT core
)

INCLUDE( CMakePackageConfigHelpers )
WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${PROJECT_BINARY_DIR}/jadematrix-ext-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)
INSTALL(
    FILES "${PROJECT_BINARY_DIR}/jadematrix-ext-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/JadeMatrix-ext/"
    COMPONENT cmake
)

INSTALL(
    EXPORT "jadematrix-ext-config"
    NAMESPACE "JadeMatrix::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/JadeMatrix-ext/"
    COMPONENT cmake
)

EXPORT( EXPORT "jadematrix-ext-config" FILE "jadematrix-ext-config.cmake" )


IF( BUILD_TESTING )
    ADD_SUBDIRECTORY( "tests/" )
ENDIF()
