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
    string(JOIN " " CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS}
           -L/usr/local/lib -rdynamic)
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

message("::: ${IniParser_INCLUDE_DIRS}")

include_directories("/usr/local/include/antlr4-runtime")

###############################################################################
# Symbol and header checks

check_symbol_exists (err            "err.h"      HAVE_ERR)
check_symbol_exists (posix_spawnp   "spawn.h"    HAVE_POSIX_SPAWNP)
check_symbol_exists (arc4random     "stdlib.h"   HAVE_ARC4RANDOM)
check_symbol_exists (mkostemps      "stdlib.h"   HAVE_MKOSTEMPS)
check_symbol_exists (reallocarray   "stdlib.h"   HAVE_REALLOCARRAY)
check_symbol_exists (asprintf       "stdio.h"    HAVE_ASPRINTF)
check_symbol_exists (open_memstream "stdio.h"    HAVE_OPEN_MEMSTREAM)
check_symbol_exists (vasprintf      "stdio.h"    HAVE_VASPRINTF)
check_symbol_exists (memrchr        "string.h"   HAVE_MEMRCHR)
check_symbol_exists (strchrnul      "string.h"   HAVE_STRCHRNUL)
check_symbol_exists (strdupa        "string.h"   HAVE_STRDUPA)
check_symbol_exists (stricmp        "string.h"   HAVE_STRICMP)
check_symbol_exists (strlcat        "string.h"   HAVE_STRLCAT)
check_symbol_exists (strlcpy        "string.h"   HAVE_STRLCPY)
check_symbol_exists (strsep         "string.h"   HAVE_STRSEP)
check_symbol_exists (strtonum       "string.h"   HAVE_STRTONUM)
check_symbol_exists (strcasecmp     "strings.h"  HAVE_STRCASECMP)
check_symbol_exists (gettimeofday   "sys/time.h" HAVE_GETTIMEOFDAY)
check_symbol_exists (clock_gettime  "time.h"     HAVE_CLOCK_GETTIME)
check_symbol_exists (fork           "unistd.h"   HAVE_FORK)
check_symbol_exists (pause          "unistd.h"   HAVE_PAUSE)
check_symbol_exists (pipe2          "unistd.h"   HAVE_PIPE2)

check_include_file ("execinfo.h"    HAVE_HEADER_EXECINFO_H)
check_include_file ("socket.h"      HAVE_HEADER_SOCKET_H)
check_include_file ("stdatomic.h"   HAVE_HEADER_STDATOMIC_H)
check_include_file ("stdnoreturn.h" HAVE_HEADER_STDNORETURN_H)
check_include_file ("sys/auxv.h"    HAVE_HEADER_SYS_AUXV_H)
check_include_file ("sys/socket.h"  HAVE_HEADER_SYS_SOCKET_H)
check_include_file ("threads.h"     HAVE_HEADER_THREADS_H)
check_include_file ("sys/single_threaded.h" HAVE_HEADER_SYS_SINGLE_THREADED_H)


###############################################################################
# General flags

string(JOIN " " CMAKE_C_FLAGS ${LIBCLANG_CFLAGS} ${CMAKE_C_FLAGS})

string (JOIN " " WARNS
        -Wall -Wextra -Wpedantic
        -Werror=format-extra-args
        #-Werror=implicit
        #-Werror=implicit-function-declaration
        #-Werror=incompatible-pointer-types
        #-Werror=int-conversion
        #-Werror=pointer-to-int-cast
)

if (NOT BUILD_DIST) 
    set (MARCH_SETTING "-march=native")
endif()

if (SANITIZE)
    set (SANIT "-fsanitize-address-use-after-scope -fsanitize=undefined")
    string(CONCAT SANITIZER_LOG_PLACE $ENV{HOME} "/sanit/asan.log")
    if ("${SANITIZE}" STREQUAL "thread")
        string (CONCAT SANIT "${SANIT} -fsanitize=thread")
    else ()
        string (CONCAT SANIT "${SANIT} -fsanitize=address")
    endif()
endif()

message(WARNING "Sanitize set to \"${SANIT}\" -- \"${SANITIZE}\"")

string (CONCAT BASE "${WARNS} ${MARCH_SETTING} ${SANIT}"
                    " -pipe -fdiagnostics-color=always -m64")

################################################################################
# Compiler specific flags. Currently these override $CFLAGS.

string(FIND ${CMAKE_C_COMPILER} "clang" CLANG_STR_SEARCH)
if (NOT CLANG_STR_SEARCH EQUAL -1)
    set (CC_IS_CLANG 1)
endif()

set (CFLAGS_DEBUG_COMMON          "-Og -g3 -gdwarf-5 -Wextra -Wpedantic -Wformat")
set (CFLAGS_RELWITHDEBINFO_COMMON "-O2 -g  -Wextra -ftree-vectorize -Wextra")
set (CFLAGS_RELEASE_COMMON        "-O3 -g0 -DNDEBUG -ffunction-sections -U_FORTIFY_SOURCE")

if (SANITIZE)
    string (JOIN " " CFLAGS_DEBUG_COMMON ${CFLAGS_DEBUG_COMMON}
            -fno-omit-frame-pointer -fno-optimize-sibling-calls)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    string(CONCAT CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    string (JOIN " " WARNS ${WARNS}
         -Weverything
         -Wno-c++98-compat -Wno-documentation -Wno-padded -Wno-suggest-destructor-override
         -Wno-inconsistent-missing-destructor-override -Wno-c++98-compat-pedantic -Wno-documentation-unknown-command
         -Wno-shadow-field -Wno-extra-semi -Wno-exit-time-destructors -Wno-weak-vtables
         -Wno-reserved-id-macro
         -Wno-gnu -Wno-gnu-zero-variadic-macro-arguments 
         -Wno-gnu-statement-expression -Werror=return-type
         -Wno-sign-compare
    )
    string (CONCAT BASE "${BASE} ${WARNS} -fstrict-aliasing")
    #set (LTO_STR "-flto=full")

    string(CONCAT CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}"
        " -fuse-ld=lld"
        #"-flto=full -Wl,--lto-O3 -Wl,--lto-whole-program-visibility"
    )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    string (JOIN " " WARNS ${WARNS}
         # -Wsuggest-attribute=pure
         # -Wsuggest-attribute=const
         -Wsuggest-attribute=noreturn -Wsuggest-attribute=format
         -Wattributes #-Wold-style-definition -Wold-style-declaration 
    )
    string(JOIN " " BASE ${BASE} ${WARNS})
    set (LTO_STR "-flto")

    string(CONCAT CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")

elseif (MSVC)
   # Allow use of deprecated function names in MSVC (read/write)
   add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

#####################################################################################################
# Misc

string(JOIN " " CMAKE_C_FLAGS_DEBUG
                ${CMAKE_C_FLAGS_DEBUG} ${CFLAGS_DEBUG_COMMON} ${BASE} ${CMAKE_C_FLAGS})
string(JOIN " " CMAKE_C_FLAGS_RELWITHDEBINFO 
                ${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CFLAGS_RELWITHDEBINFO_COMMON} ${BASE} ${LTO_STR} ${CMAKE_C_FLAGS})
string(JOIN " " CMAKE_C_FLAGS_RELEASE
                ${CMAKE_C_FLAGS_RELEASE} ${CFLAGS_RELEASE_COMMON} ${BASE} ${LTO_STR} ${CMAKE_C_FLAGS})
string(JOIN " " CMAKE_C_FLAGS_MINSIZEREL
                ${CMAKE_C_FLAGS_MINSIZEREL} ${CFLAGS_RELEASE_COMMON} ${BASE} -Os -s ${CMAKE_C_FLAGS})

string(JOIN " " CMAKE_CXX_FLAGS_DEBUG           ${CMAKE_C_FLAGS_DEBUG}          -std=gnu++${CMAKE_CXX_STANDARD} ${CMAKE_CXX_FLAGS})
string(JOIN " " CMAKE_CXX_FLAGS_RELEASE         ${CMAKE_C_FLAGS_RELEASE}        -std=gnu++${CMAKE_CXX_STANDARD} ${CMAKE_CXX_FLAGS})
string(JOIN " " CMAKE_CXX_FLAGS_RELWITHDEBINFO  ${CMAKE_C_FLAGS_RELWITHDEBINFO} -std=gnu++${CMAKE_CXX_STANDARD} ${CMAKE_CXX_FLAGS})
string(JOIN " " CMAKE_CXX_FLAGS_MINSIZEREL      ${CMAKE_C_FLAGS_MINSIZEREL}     -std=gnu++${CMAKE_CXX_STANDARD} ${CMAKE_CXX_FLAGS})

if (MINGW)
    FIX_WINDOWS_PATHS(CMAKE_C_FLAGS)
endif()

configure_file(cmake-config.h.in config.h)

include_directories(/usr/include/libxml2)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/Gen)

add_subdirectory(src)
