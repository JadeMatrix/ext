# `JadeMatrix::ext::` — C++ Standard Library Extension/Interop/Synergy Utilities

Requires C++17 or higher.  Unit tests require C++20, [`doctest`](https://github.com/onqtam/doctest) v2.3.3+, and [`JadeMatrix-CMake-Modules`](https://github.com/JadeMatrix/JadeMatrix-CMake-Modules).

This library uses [`fmt`](https://github.com/fmtlib/fmt) in place of [`<format>`](https://en.cppreference.com/w/cpp/header/format) as at this time there are no mature implementations of the latter.  This can be overridden by configuring with the CMake variable `JM_EXT_USE_STD_FORMAT` set to true.  This will also set `JM_EXT_USE_STD_FORMAT` as an `INTERFACE` compile definition on the target; if CMake isn't used for a project using this library, the `JM_EXT_USE_STD_FORMAT` definition will have to be passed to the compiler manually.
