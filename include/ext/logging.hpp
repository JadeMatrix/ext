#pragma once


// A simple, extensible logging framework with no macros required

#include <ext/source_location.hpp>

#ifdef JM_EXT_USE_STD_FORMAT
    #ifndef __cpp_lib_format
        #error JM_EXT_USE_STD_FORMAT defined but <format> not available
    #endif
    #include <format>
    #define JM_EXT_FMT_NAMESPACE std
#else
    #include <fmt/format.h>
    #define JM_EXT_FMT_NAMESPACE fmt
#endif

#include <chrono>
#include <functional>   // function
#include <string>
#include <string_view>
#include <utility>      // move


namespace JadeMatrix::ext::log
{
    enum class level
    {
        verbose,
        info,
        warning,
        error
    };
    
    namespace internal
    {
        struct format
        {
            std::string_view                      string;
            std::chrono::system_clock::time_point when;
            ext::source_location                  location;
            
            constexpr format(
                std::string_view                      str,
                std::chrono::system_clock::time_point now = std::chrono::system_clock::now(),
                ext::source_location                  loc = ext::source_location::current()
            ) : string( str ), when( now ), location( loc )
            {}
            
            [[deprecated( "use string_view literals" )]] constexpr format(
                char const*                           str,
                std::chrono::system_clock::time_point now = std::chrono::system_clock::now(),
                ext::source_location                  loc = ext::source_location::current()
            ) : format( std::string_view( str ), now, loc )
            {}
        };
    }
    
    struct settings
    {
        std::function< bool( level ) > should_emit_for;
        std::function< void(
            level                                 lvl,
            std::chrono::system_clock::time_point now,
            ext::source_location                  loc,
            std::string&&                         msg
        ) > emit;
    };
    
    class handle
    {
    public:
        handle( settings s ) : settings_( std::move( s ) ) {}
        
        template< typename... Args > constexpr void as(
            level            as_level,
            internal::format with_format,
            Args&&...        args
        ) const
        {
            if( settings_.should_emit_for( as_level ) )
            {
                settings_.emit(
                    as_level,
                    with_format.when,
                    with_format.location,
                    JM_EXT_FMT_NAMESPACE::format(
                        with_format.string,
                        std::forward< Args >( args )...
                    )
                );
            }
        }
        
        #define DEFINE_LOG_FUNCTION( LEVEL ) \
            template< typename... Args > constexpr void LEVEL( \
                internal::format with_format, \
                Args&&...        args \
            ) const \
            { \
                as( \
                    level::LEVEL, \
                    with_format, \
                    std::forward< Args >( args )... \
                ); \
            }
        DEFINE_LOG_FUNCTION(verbose)
        DEFINE_LOG_FUNCTION(info   )
        DEFINE_LOG_FUNCTION(warning)
        DEFINE_LOG_FUNCTION(error  )
        #undef DEFINE_LOG_FUNCTION
        
    private:
        settings settings_;
    };
}
