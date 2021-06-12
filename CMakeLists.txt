cmake_minimum_required (VERSION 3.20.2)
project (x4c C CXX)
include (FindPkgConfig)
include (CheckFunctionExists)
include (CheckSymbolExists)
include (CheckIncludeFile)

###############################################################################
# Options

option(SANITIZE "Enable sanitizers" OFF)
option(USE_JEMALLOC "Use jemalloc" OFF)
option(USE_LIBEV "Use libev for the event loop (if available)" "TRY")

macro(FIX_WINDOWS_PATHS _pathvar)
    string(REPLACE "\\" "/" ${_pathvar} "${${_pathvar}}")
endmacro()

if (NOT CMAKE_BUILD_TYPE)
    message("Setting build type to \"Release\"")
    set (CMAKE_BUILD_TYPE "Release")
endif()

if ((CMAKE_BUILD_TYPE STREQUAL "Debug") OR
    (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    set (DEBUG 1)
endif()

###############################################################################
# Includes and defines

if (MINGW)
    add_definitions(-D__MINGW__)
    add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
else()
    string (JOIN " " CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}"
            "-L/usr/local/lib" "-L/usr/local/lib64" "-rdynamic")
endif()

add_definitions(-D_GNU_SOURCE -DHAVE_CONFIG_H -DBSTR_USE_P99 -DBSTR_USE_TALLOC)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD})
set (CMAKE_REQUIRED_DEFINITIONS -D__USE_ISOC99 -D__USE_ISOC11 -D__USE_ISOCXX11
     -D_GNU_SOURCE -DHAVE_CONFIG_H)

###############################################################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

find_package (Threads REQUIRED)
find_package (IniParser REQUIRED)
find_package (fmt REQUIRED)
if (USE_JEMALLOC)
    find_package (Jemalloc REQUIRED)
endif()

include_directories("/usr/local/include/antlr4-runtime")

###############################################################################
# Symbol and header checks

CHECK_INCLUDE_FILE ("time.h"     HAVE_TIME_H)
CHECK_INCLUDE_FILE ("sys/time.h" HAVE_SYS_TIME_H)

check_symbol_exists (err            "err.h"      HAVE_ERR)
CHECK_SYMBOL_EXISTS (posix_spawnp   "spawn.h"    HAVE_POSIX_SPAWNP)
CHECK_SYMBOL_EXISTS (arc4random     "stdlib.h"   HAVE_ARC4RANDOM)
CHECK_SYMBOL_EXISTS (mkostemps      "stdlib.h"   HAVE_MKOSTEMPS)
CHECK_SYMBOL_EXISTS (reallocarray   "stdlib.h"   HAVE_REALLOCARRAY)
CHECK_SYMBOL_EXISTS (strtonum       "stdlib.h"   HAVE_STRTONUM)
CHECK_SYMBOL_EXISTS (asprintf       "stdio.h"    HAVE_ASPRINTF)
CHECK_SYMBOL_EXISTS (dprintf        "stdio.h"    HAVE_DPRINTF)
CHECK_SYMBOL_EXISTS (open_memstream "stdio.h"    HAVE_OPEN_MEMSTREAM)
CHECK_SYMBOL_EXISTS (vasprintf      "stdio.h"    HAVE_VASPRINTF)
CHECK_SYMBOL_EXISTS (memrchr        "string.h"   HAVE_MEMRCHR)
CHECK_SYMBOL_EXISTS (strchrnul      "string.h"   HAVE_STRCHRNUL)
CHECK_SYMBOL_EXISTS (strdupa        "string.h"   HAVE_STRDUPA)
CHECK_SYMBOL_EXISTS (stricmp        "string.h"   HAVE_STRICMP)
CHECK_SYMBOL_EXISTS (strlcat        "string.h"   HAVE_STRLCAT)
CHECK_SYMBOL_EXISTS (strlcpy        "string.h"   HAVE_STRLCPY)
CHECK_SYMBOL_EXISTS (strsep         "string.h"   HAVE_STRSEP)
CHECK_SYMBOL_EXISTS (strcasecmp     "strings.h"  HAVE_STRCASECMP)
CHECK_SYMBOL_EXISTS (fork           "unistd.h"   HAVE_FORK)
CHECK_SYMBOL_EXISTS (pause          "unistd.h"   HAVE_PAUSE)
CHECK_SYMBOL_EXISTS (pipe2          "unistd.h"   HAVE_PIPE2)

if (HAVE_TIME_H)
    CHECK_SYMBOL_EXISTS (clock_gettime  "time.h"     HAVE_CLOCK_GETTIME)
    CHECK_SYMBOL_EXISTS (gettimeofday   "time.h"     HAVE_GETTIMEOFDAY)
    CHECK_SYMBOL_EXISTS (nanosleep      "time.h"     HAVE_NANOSLEEP)
elseif (HAVE_SYS_TIME_H)
    CHECK_SYMBOL_EXISTS (clock_gettime  "sys/time.h" HAVE_CLOCK_GETTIME)
    CHECK_SYMBOL_EXISTS (gettimeofday   "sys/time.h" HAVE_GETTIMEOFDAY)
    CHECK_SYMBOL_EXISTS (nanosleep      "sys/time.h" HAVE_NANOSLEEP)
else()
    CHECK_SYMBOL_EXISTS (clock_gettime  "pthread.h"  HAVE_CLOCK_GETTIME)
    CHECK_SYMBOL_EXISTS (gettimeofday   "pthread.h"  HAVE_GETTIMEOFDAY)
    CHECK_SYMBOL_EXISTS (nanosleep      "pthread.h"  HAVE_NANOSLEEP)
endif()

# CHECK_INCLUDE_FILE ("bsd/stdlib.h"  HAVE_BSD_STDLIB_H)
CHECK_INCLUDE_FILE ("execinfo.h"    HAVE_EXECINFO_H)
CHECK_INCLUDE_FILE ("stdatomic.h"   HAVE_STDATOMIC_H)
CHECK_INCLUDE_FILE ("stdnoreturn.h" HAVE_STDNORETURN_H)
CHECK_INCLUDE_FILE ("sys/auxv.h"    HAVE_SYS_AUXV_H)
CHECK_INCLUDE_FILE ("threads.h"     HAVE_THREADS_H)

###############################################################################
# General flags

set (CMAKE_C_FLAGS ${LIBCLANG_CFLAGS} ${CMAKE_C_FLAGS})

set (WARNS
        -Wall -Wextra -Wpedantic
        -Werror=format-extra-args
        -Werror=invalid-pch
        #-Werror=implicit
        #-Werror=implicit-function-declaration
        #-Werror=incompatible-pointer-types
        #-Werror=int-conversion
        #-Werror=pointer-to-int-cast
)

if (NOT BUILD_DIST) 
    set (MARCH_SETTING -march=native)
endif()

if (SANITIZE)
    set (SANIT -fsanitize-address-use-after-scope -fsanitize=undefined -fsanitize=bounds -fsanitize=bool)
    if ("${SANITIZE}" STREQUAL "thread")
        set (SANIT ${SANIT} -fsanitize=thread)
    elseif ("${SANITIZE}" STREQUAL "memory")
        set (SANIT -fsanitize=undefined -fsanitize=memory)
    else ()
        set (SANIT ${SANIT} -fsanitize=address)
    endif()
endif()

message(WARNING "Sanitize set to \"${SANIT}\" -- \"${SANITIZE}\"")

set (BASE ${WARNS} ${MARCH_SETTING} ${SANIT}
          -pipe -fdiagnostics-color=always -m64)

################################################################################
# Compiler specific flags. Currently these override $CFLAGS.

string(FIND ${CMAKE_C_COMPILER} "clang" CLANG_STR_SEARCH)
if (NOT CLANG_STR_SEARCH EQUAL -1)
    set (CC_IS_CLANG 1)
endif()

set (CFLAGS_DEBUG_COMMON          -Og -g3 -Wextra -Wpedantic -Wformat)
set (CFLAGS_RELWITHDEBINFO_COMMON -O2 -g  -Wextra -ftree-vectorize -Wextra)
set (CFLAGS_RELEASE_COMMON        -O3 -g0 -DNDEBUG -ffunction-sections -U_FORTIFY_SOURCE)

if (SANITIZE)
    set (CFLAGS_DEBUG_COMMON ${CFLAGS_DEBUG_COMMON}
         -fno-omit-frame-pointer -fno-optimize-sibling-calls)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (WARNS ${WARNS}
         # -Weverything
         # -Wno-c++98-compat -Wno-documentation -Wno-padded -Wno-suggest-destructor-override
         # -Wno-inconsistent-missing-destructor-override -Wno-c++98-compat-pedantic -Wno-documentation-unknown-command
         # -Wno-shadow-field -Wno-extra-semi -Wno-exit-time-destructors -Wno-weak-vtables
         # -Wno-reserved-id-macro
         -Wno-gnu -Wno-gnu-zero-variadic-macro-arguments 
         -Wno-gnu-statement-expression -Werror=return-type
         -Wno-sign-compare
    )
    set (BASE ${BASE} ${WARNS}
        -Qunused-arguments
        "-fstrict-aliasing" -rtlib=compiler-rt -stdlib=libstdc++
        #"-include ${CMAKE_SOURCE_DIR}/src/_common.hh"
        #"-isystem /usr/include/c++/v1"
        #"-fno-PIC" "-fno-pie" #-stdlib=libc++
    )
    set (LTO_STR "-flto=full")

    string (JOIN " " CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}"
            -fuse-ld=lld -rtlib=compiler-rt #-stdlib=libc++
        #"-flto=full -Wl,--lto-O3 -Wl,--lto-whole-program-visibility"
    )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    set (WARNS ${WARNS}
         # -Wsuggest-attribute=pure
         # -Wsuggest-attribute=const
         -Wsuggest-attribute=noreturn -Wsuggest-attribute=format
         -Wattributes #-Wold-style-definition -Wold-style-declaration 
    )
    set (BASE ${BASE} ${WARNS} -fPIC -fpie
        "-include ${CMAKE_SOURCE_DIR}/src/_common.hh"
    )
    set (LTO_STR "-flto")

    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")

elseif (MSVC)
   # Allow use of deprecated function names in MSVC (read/write)
   add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

#####################################################################################################
# Misc

if (MINGW)
    FIX_WINDOWS_PATHS(CMAKE_C_FLAGS)
endif()

set(CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG}
        ${CFLAGS_DEBUG_COMMON} ${BASE} ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO}
        ${CFLAGS_RELWITHDEBINFO_COMMON} ${BASE} ${LTO_STR} ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE}
        ${CFLAGS_RELEASE_COMMON} ${BASE} ${LTO_STR} ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL}
        ${CFLAGS_RELEASE_COMMON} ${BASE} -Os -s ${CMAKE_C_FLAGS})
   
set(CMAKE_CXX_FLAGS -std=gnu++${CMAKE_CXX_STANDARD} ${CMAKE_CXX_FLAGS})
    
set(CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_C_FLAGS_DEBUG}          ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_C_FLAGS_RELEASE}        ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_MINSIZEREL     ${CMAKE_C_FLAGS_MINSIZEREL}     ${CMAKE_CXX_FLAGS})

# string(TOUPPER ${CMAKE_BUILD_TYPE} _upper_CMAKE_BUILD_TYPE)
# set (ALL_THE_C_FLAGS   ${CMAKE_C_FLAGS_${_upper_CMAKE_BUILD_TYPE}})
# set (ALL_THE_CXX_FLAGS ${CMAKE_CXX_FLAGS_${_upper_CMAKE_BUILD_TYPE}})
# message("THE FLAGS:\t\t\"${ALL_THE_CXX_FLAGS}\"")

string(JOIN " " CMAKE_C_FLAGS_DEBUG             ${CMAKE_C_FLAGS_DEBUG})
string(JOIN " " CMAKE_C_FLAGS_RELEASE           ${CMAKE_C_FLAGS_RELEASE})
string(JOIN " " CMAKE_C_FLAGS_RELWITHDEBINFO    ${CMAKE_C_FLAGS_RELWITHDEBINFO})
string(JOIN " " CMAKE_C_FLAGS_MINSIZEREL        ${CMAKE_C_FLAGS_MINSIZEREL})
string(JOIN " " CMAKE_CXX_FLAGS_DEBUG           ${CMAKE_CXX_FLAGS_DEBUG})
string(JOIN " " CMAKE_CXX_FLAGS_RELEASE         ${CMAKE_CXX_FLAGS_RELEASE})
string(JOIN " " CMAKE_CXX_FLAGS_RELWITHDEBINFO  ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
string(JOIN " " CMAKE_CXX_FLAGS_MINSIZEREL      ${CMAKE_CXX_FLAGS_MINSIZEREL})

#####################################################################################################

configure_file(cmake-config.h.in config.h)

include_directories(/usr/include/libxml2)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/Gen)

add_subdirectory(src)
