#pragma once

// Debug-only console print wrappers, forwards to printf/puts etc.
// Don't put any expression with side effects inside, like myprintf("%d", ++i);
// since expressions are dropped on Release build

#ifdef _DEBUG
    #include <cstdio>
    #define myprintf(...) std::printf(__VA_ARGS__)
    #define myputs(str) std::puts(str)
    #define mywprintf(...) std::wprintf(__VA_ARGS__)
#else
    #define myprintf(...) ((void)0)
    #define myputs(str) ((void)0)
    #define mywprintf(...) ((void)0)
#endif

#ifdef _DEBUG
    #include <print>
    #define myprint(...) std::print(__VA_ARGS__)
    #define myprintln(...) std::println(__VA_ARGS__)
#else
    #define myprint(...) ((void)0)
    #define myprintln(...) ((void)0)
#endif

//script style print
#ifdef _DEBUG
    #include <iostream>
namespace utils::dbg {
    template<typename... Args>
    inline void pyprint_impl(const Args&... args) {
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
    }
    //wc
    template<typename... Args>
    inline void wpyprint_impl(const Args&... args) {
        ((std::wcout << args << L' '), ...);
        std::wcout << L'\n';
    }
}
    #define pyprint(...) utils::dbg::pyprint_impl(__VA_ARGS__)
    #define wpyprint(...) utils::dbg::wpyprint_impl(__VA_ARGS__)
#else
    #define pyprint(...) ((void)0)
    #define wpyprint(...) ((void)0)
#endif