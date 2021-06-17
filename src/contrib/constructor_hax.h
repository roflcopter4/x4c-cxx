#pragma once

// Initializer/finalizer sample for MSVC and GCC/Clang.
// 2010-2016 Joe Lowe. Released into the public domain.

/*
 * Edited somewhat by some idiot called RoflCopter4.
 * Now _requires_ a conforment C pre-processor and the
 * common extension macro `__COUNTER__'.
 */

#if defined(__GNUC__) || defined(__clang__)
#  define HELP_HAX_INITIALIZER_1_(FN, CNT) \
        __attribute__((__constructor__)) static void FN##_##CNT(void)
#elif defined __cplusplus
#  define HELP_HAX_INITIALIZER_2_(FN, ST)         \
        static void FN(void);                     \
        struct ST##t_ { ST##t_(void) { FN(); } }; \
        static ST##t_ ST;                         \
        static void FN(void)

#  define HELP_HAX_INITIALIZER_1_(FN, CNT) \
      HELP_HAX_INITIALIZER_2_(FN##_##CNT, FN##_cxx_run_at_program_init_hack_##CNT)
#elif defined(_MSC_VER)
#  pragma section(".CRT$XCU", read)
#  define HELP_HAX_INITIALIZER_2_(fn, p)                           \
        static void fn(void);                                      \
        __declspec(allocate(".CRT$XCU")) void (*fn##_)(void) = fn; \
        __pragma(comment(linker, "/include:" p #fn "_"))           \
        static void f(void)

#  ifdef _WIN64
#    define HELP_HAX_INITIALIZER_1_(FN, CNT) \
            HELP_HAX_INITIALIZER_2_(FN##_##CNT, "")
#  else
#    define HELP_HAX_INITIALIZER_1_(FN, CNT) \
            HELP_HAX_INITIALIZER_2_(FN##_##CNT, "_")
#  endif
#else
#  error "Are you using tcc or something? Sorry, your oddball compiler isn't supported."
#endif

#define HELP_HAX_INITIALIZER_0_(...) HELP_HAX_INITIALIZER_1_(__VA_ARGS__)
#define HAX_INITIALIZER(FN)          HELP_HAX_INITIALIZER_0_(FN, __COUNTER__)

