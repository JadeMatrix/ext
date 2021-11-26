CMAKE_MINIMUM_REQUIRED( VERSION 3.1 FATAL_ERROR #[[
    Requiring 3.1+:
        WRITE_BASIC_PACKAGE_VERSION_FILE( ... )
]] )

PROJECT( "JadeMatrix-ext"
    VERSION   0.1
    LANGUAGES CXX
)


SET( CMAKE_CXX_STANDARD          17  )
SET( CMAKE_CXX_STANDARD_REQUIRED ON  )
SET( CMAKE_CXX_EXTENSIONS        OFF )

INCLUDE( CTest          )
INCLUDE( GNUInstallDirs )


# Bootstrap methods for finding dependencies ###################################

IF( EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
    INCLUDE( "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
    CONAN_SET_FIND_PATHS()
ELSE()
    LIST( APPEND CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/dependencies/JadeMatrix/JadeMatrix-CMake-Modules/CMakeModules/"
    )
ENDIF()

INCLUDE( JadeMatrix-CMake-Modules/Util/AddDependency
    OPTIONAL
    RESULT_VARIABLE JMCMM_INCLUDED
)
IF( NOT JMCMM_INCLUDED )
    FIND_PACKAGE( JadeMatrix-CMake-Modules 1.7 REQUIRED )
    INCLUDE( JadeMatrix-CMake-Modules/Util/AddDependency )
ENDIF()


# Find dependencies ############################################################

IF( BUILD_TESTING )
    ADD_DEPENDENCY( doctest
        REQUIRED
        VERSION      2.3.3
        SUBDIRECTORY "dependencies/onqtam/doctest/"
        COMPONENTS   doctest
        PROPAGATE    CMAKE_MODULE_PATH
    )
    GET_TARGET_PROPERTY( DOCTEST_IS_IMPORTED doctest::doctest IMPORTED )
    IF( NOT DOCTEST_IS_IMPORTED )
        LIST( APPEND CMAKE_MODULE_PATH
            "${PROJECT_SOURCE_DIR}/dependencies/onqtam/doctest/scripts/cmake/"
        )
    ENDIF()
ENDIF()


# Set up project targets #######################################################

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


# Set up project tests #########################################################

IF( BUILD_TESTING )
    ADD_SUBDIRECTORY( "tests/" )
ENDIF()


# Set up project exports & installation ########################################

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
