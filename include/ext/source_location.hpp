#pragma once


// An extension of C++20's `std::source_location` with mutable fields, important
// for interfacing with older libraries and interpreted/non-C++ languages

#ifdef __cpp_lib_source_location
#include <source_location>
#endif

#include <cstdint>  // uint_least32_t


namespace JadeMatrix::ext
{
    struct source_location
    {
        std::uint_least32_t line          = 0u;
        std::uint_least32_t column        = 0u;
        char const        * file_name     = "";
        char const        * function_name = "";
        
        constexpr source_location(
            std::uint_least32_t line_,
            std::uint_least32_t column_,
            char const        * file_name_,
            char const        * function_name_
        ) noexcept;
        constexpr source_location(                        ) noexcept = default;
        constexpr source_location( source_location const& ) noexcept = default;
        constexpr source_location( source_location     && ) noexcept = default;
        
        #ifdef __cpp_lib_source_location
        constexpr source_location( std::source_location ) noexcept;
        #endif
        
        static
        #ifdef __cpp_consteval
            consteval
        #else
            constexpr
        #endif
        source_location current(
            #ifdef __cpp_lib_source_location
                // TODO: Test that this actually works
                std::source_location orig = std::source_location::current()
            #else
                #if defined( __GNUG__ )
                    // GCC & Clang
                    #if __has_builtin( __builtin_LINE )
                        std::uint_least32_t line_ = __builtin_LINE(),
                    #else
                        std::uint_least32_t line_ = 0u,
                    #endif
                    #if __has_builtin( __builtin_COLUMN )
                        std::uint_least32_t column_ = __builtin_COLUMN(),
                    #else
                        std::uint_least32_t column_ = 0u,
                    #endif
                    #if __has_builtin( __builtin_FILE )
                        char const * file_name_ = __builtin_FILE(),
                    #else
                        char const * file_name_ = "",
                    #endif
                    #if __has_builtin( __builtin_FUNCTION )
                        char const * function_name_ = __builtin_FUNCTION()
                    #else
                        char const * function_name_ = ""
                    #endif
                #else
                    // Using `__LINE__` etc. doesn't work as they resolve to
                    // this location, not the called location
                    std::uint_least32_t line_          = 0u,
                    std::uint_least32_t column_        = 0u,
                    char const        * file_name_     = "",
                    char const        * function_name_ = ""
                #endif
            #endif
        ) noexcept;
    };
}


constexpr JadeMatrix::ext::source_location::source_location(
    std::uint_least32_t line_,
    std::uint_least32_t column_,
    char const        * file_name_,
    char const        * function_name_
) noexcept :
    line         ( line_          ),
    column       ( column_        ),
    file_name    ( file_name_     ),
    function_name( function_name_ )
{}

#ifdef __cpp_lib_source_location
constexpr JadeMatrix::ext::source_location::source_location(
    std::source_location orig
) noexcept :
    line         ( orig.line()          ),
    column       ( orig.column()        ),
    file_name    ( orig.file_name()     ),
    function_name( orig.function_name() )
{}
#endif

#ifdef __cpp_consteval
    consteval
#else
    constexpr
#endif
JadeMatrix::ext::source_location JadeMatrix::ext::source_location::current(
    #ifdef __cpp_lib_source_location
        std::source_location orig
    #else
        std::uint_least32_t line_,
        std::uint_least32_t column_,
        char const        * file_name_,
        char const        * function_name_
    #endif
) noexcept
{
    #ifdef __cpp_lib_source_location
        return orig;
    #else
        return { line_, column_, file_name_, function_name_ };
    #endif
}
