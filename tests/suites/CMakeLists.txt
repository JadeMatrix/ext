CMAKE_MINIMUM_REQUIRED( VERSION 3.11 FATAL_ERROR #[[
    Requiring 3.11+:
        ADD_*( foo ) ... TARGET_SOURCES( foo ... )
    Requiring 3.10+:
        INCLUDE( doctest )
]] )

INCLUDE( doctest )


# Regular runnable unit tests
IF( NOT TARGET tests )
    ADD_CUSTOM_TARGET( tests ALL )
ENDIF()

FOREACH( CPP_VERSION IN ITEMS "17" "20" )
    FOREACH( SUITE IN ITEMS
        "logging"
        "source_location"
    )
        ADD_EXECUTABLE( ${SUITE}_tests_${CPP_VERSION} )
        TARGET_SOURCES( ${SUITE}_tests_${CPP_VERSION} PRIVATE "${SUITE}.cpp" )
        TARGET_LINK_LIBRARIES( ${SUITE}_tests_${CPP_VERSION}
            PRIVATE
                ext
                doctest::doctest
        )
        IF( NOT JM_EXT_USE_STD_FORMAT )
            TARGET_LINK_LIBRARIES( ${SUITE}_tests_${CPP_VERSION}
                PRIVATE fmt::fmt
            )
        ENDIF()
        SET_ALL_COMPILE_WARNINGS( ${SUITE}_tests_${CPP_VERSION} AS_ERRORS )
        TARGET_COMPILE_DEFINITIONS( ${SUITE}_tests_${CPP_VERSION}
            PRIVATE
                "JM_EXT_TEST_CPP_VERSION_STRING=\"C++${CPP_VERSION}\""
        )
        SET_TARGET_PROPERTIES( ${SUITE}_tests_${CPP_VERSION}
            PROPERTIES
                CXX_STANDARD          ${CPP_VERSION}
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS        OFF
        )
        
        ADD_DEPENDENCIES( tests ${SUITE}_tests_${CPP_VERSION} )
        DOCTEST_DISCOVER_TESTS( ${SUITE}_tests_${CPP_VERSION} )
    ENDFOREACH()
ENDFOREACH()
