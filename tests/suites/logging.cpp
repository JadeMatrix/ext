#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ext/logging.hpp>

#include <sstream>
#include <string_view>


namespace ext = ::JadeMatrix::ext;


namespace
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    
    struct formatted_checker
    {
        bool mutable was_formatted = false;
    };
}

template<> struct JM_EXT_FMT_NAMESPACE::formatter< formatted_checker >
{
    constexpr auto parse( format_parse_context& ctx )
    {
        auto iter = ctx.begin();
        while( iter != ctx.end() )
        {
            if( *( iter++ ) == '}' ) return iter;
        }
        return iter;
    }
    
    template< typename FormatContext > constexpr auto format(
        formatted_checker const& v,
        FormatContext          & ctx
    )
    {
        v.was_formatted = true;
        return format_to( ctx.out(), "was formatted"sv );
    }
};


#define EMIT_OUTPUT_TO_STRING_CASE( LEVEL ) \
    TEST_CASE( \
        "ext::log::handle output to string (" \
        #LEVEL \
        ") (" \
        JM_EXT_TEST_CPP_VERSION_STRING \
        ")" \
    ) \
    { \
        std::stringstream ss; \
         \
        const ext::log::settings settings{ \
            /* should_emit_for = */ []( ext::log::level ){ return true; }, \
            /* emit = */ [ & ss ]( \
                ext::log::level                       lvl, \
                std::chrono::system_clock::time_point /* now */, \
                ext::source_location                  /* loc */, \
                std::string&&                         msg \
            ) \
            { \
                switch( lvl ) \
                { \
                case ext::log::level::verbose: ss << "verbose: "sv; break; \
                case ext::log::level::info   : ss << "info: "sv   ; break; \
                case ext::log::level::warning: ss << "warning: "sv; break; \
                case ext::log::level::error  : ss << "error: "sv  ; break; \
                } \
                ss << msg; \
            } \
        }; \
         \
        const auto handle = ext::log::handle( settings ); \
         \
        handle.LEVEL( "hello world"sv ); \
         \
        REQUIRE( ss.str() == #LEVEL ": hello world"sv ); \
    }
EMIT_OUTPUT_TO_STRING_CASE( verbose )
EMIT_OUTPUT_TO_STRING_CASE( info    )
EMIT_OUTPUT_TO_STRING_CASE( warning )
EMIT_OUTPUT_TO_STRING_CASE( error   )
#undef EMIT_OUTPUT_TO_STRING_CASE


#define EMIT_CHECK_LEVEL_CASE( LEVEL ) \
    TEST_CASE( \
        "ext::log::handle check level > " \
        #LEVEL \
        " (" \
        JM_EXT_TEST_CPP_VERSION_STRING \
        ")" \
    ) \
    { \
        std::stringstream ss; \
         \
        const ext::log::settings settings{ \
            /* should_emit_for = */ []( ext::log::level lvl ){ \
                return lvl > ext::log::level::LEVEL; \
            }, \
            /* emit = */ [ & ss ]( \
                ext::log::level                       /* lvl */, \
                std::chrono::system_clock::time_point /* now */, \
                ext::source_location                  /* loc */, \
                std::string&&                         msg \
            ) \
            { \
                ss << msg; \
            } \
        }; \
         \
        const auto handle = ext::log::handle( settings ); \
         \
        handle.verbose( "hello world"sv ); \
        if constexpr( ext::log::level::verbose > ext::log::level::LEVEL ) \
            REQUIRE_FALSE( ss.str().empty() ); \
        ss.str( ""s ); \
         \
        handle.info( "hello world"sv ); \
        if constexpr( ext::log::level::info > ext::log::level::LEVEL ) \
            REQUIRE_FALSE( ss.str().empty() ); \
        ss.str( ""s ); \
         \
        handle.warning( "hello world"sv ); \
        if constexpr( ext::log::level::warning > ext::log::level::LEVEL ) \
            REQUIRE_FALSE( ss.str().empty() ); \
        ss.str( ""s ); \
         \
        handle.error( "hello world"sv ); \
        if constexpr( ext::log::level::error > ext::log::level::LEVEL ) \
            REQUIRE_FALSE( ss.str().empty() ); \
    }
EMIT_CHECK_LEVEL_CASE( verbose )
EMIT_CHECK_LEVEL_CASE( info    )
EMIT_CHECK_LEVEL_CASE( warning )
EMIT_CHECK_LEVEL_CASE( error   )
#undef EMIT_CHECK_LEVEL_CASE


TEST_CASE(
    "ext::log::handle no format if no emit ("
    JM_EXT_TEST_CPP_VERSION_STRING
    ")"
)
{
    formatted_checker checker;
    bool should_emit = false;
    
    const ext::log::settings settings{
        /* should_emit_for = */ [ & should_emit ]( ext::log::level ){
            return should_emit;
        },
        /* emit = */ [](
            ext::log::level                       /* lvl */,
            std::chrono::system_clock::time_point /* now */,
            ext::source_location                  /* loc */,
            std::string&&                         /* msg */
        ){}
    };
    
    const auto handle = ext::log::handle( settings );
    
    handle.info( "{}"sv, checker );
    REQUIRE_FALSE( checker.was_formatted );
    
    // Self-test that `formatted_checker` works
    should_emit = true;
    handle.info( "{}"sv, checker );
    REQUIRE( checker.was_formatted );
}
