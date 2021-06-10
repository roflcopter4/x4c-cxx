#pragma once

// Initializer/finalizer sample for MSVC and GCC/Clang.
// 2010-2016 Joe Lowe. Released into the public domain.

#if defined(__GNUC__) && __has_attribute(__constructor__)
#  define INITIALIZER(FN) \
        __attribute__((__constructor__)) static void FN(void)
#elif defined __cplusplus
#  define INITIALIZER(FN) INITIALIZER2_(FN, FN##_cxx_run_at_program_init_hack_)
#  define INITIALIZER2_(FN, ST)                   \
        static void FN(void);                     \
        struct ST##t_ { ST##t_(void) { FN(); } }; \
        static ST##t_ ST;                         \
        static void FN(void)
#elif defined(_MSC_VER)
#  pragma section(".CRT$XCU", read)
#  define INITIALIZER2_(fn, p)                                     \
        static void fn(void);                                      \
        __declspec(allocate(".CRT$XCU")) void (*fn##_)(void) = fn; \
        __pragma(comment(linker, "/include:" p #fn "_"))           \
        static void f(void)
#  ifdef _WIN64
#    define INITIALIZER(fn) INITIALIZER2_(fn, "")
#  else
#    define INITIALIZER(fn) INITIALIZER2_(fn, "_")
#  endif
#else
#  error "Are you using tcc or something? Sorry, your oddball compiler isn't supported."
#endif
