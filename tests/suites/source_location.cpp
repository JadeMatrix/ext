#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ext/source_location.hpp>

#include <string_view>
#include <tuple>
#ifdef __cpp_lib_source_location
#include <source_location>
#endif


namespace ext = ::JadeMatrix::ext;


namespace
{
    auto some_function_name_1()
    {
        #line 123 "CAEB1E8A-B1E5-433D-A886-9EFE627A4E7F"
        return std::tuple{                   // 123
            ext::source_location::current(), // 124
            std::string_view( __func__ )     // 125
        };
    }
    
    #ifdef __cpp_lib_source_location
    std::tuple<
        ext::source_location,
        std::string_view
    > some_function_name_2()
    {
        #line 543 "D9DF014F-8367-461B-A0A8-00AD27EC8571"
        return {                             // 534
            std::source_location::current(), // 544
            std::string_view( __func__ )     // 545
        };
    }
    #endif
}


TEST_CASE(
    "ext::source_location::curent() ("
    JM_EXT_TEST_CPP_VERSION_STRING
    ")"
)
{
    auto const [ location, function_name ] = some_function_name_1();
    
    REQUIRE( location.line == doctest::Approx( 124 ).epsilon( 2 ) );
    WARN   ( location.column == doctest::Approx( 28 ).epsilon( 15 ) );
    REQUIRE( location.file_name == "CAEB1E8A-B1E5-433D-A886-9EFE627A4E7F" );
    REQUIRE( std::string_view( location.function_name ) == function_name );
}

#ifdef __cpp_lib_source_location
TEST_CASE(
    "ext::source_location from std::source_location ("
    JM_EXT_TEST_CPP_VERSION_STRING
    ")"
)
{
    auto const [ location, function_name ] = some_function_name_2();
    
    REQUIRE( location.line == doctest::Approx( 544 ).epsilon( 2 ) );
    WARN   ( location.column == doctest::Approx( 28 ).epsilon( 15 ) );
    REQUIRE( location.file_name == "D9DF014F-8367-461B-A0A8-00AD27EC8571" );
    REQUIRE( std::string_view( location.function_name ) == function_name );
}
#endif
