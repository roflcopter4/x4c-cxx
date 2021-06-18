// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyBugproneReservedIdentifier
#pragma once
#ifndef TOP_COMMON_HH_
#define TOP_COMMON_HH_
/****************************************************************************************/

#ifdef __cplusplus

#ifndef __GNUC__
#  define __attribute__(x)
#endif
#ifndef __has_include
#  define __has_include(x)
#endif

#if 0 && __cplusplus >= 201700L || defined _MSC_VER
#  define UNUSED [[maybe_unused]]
#  define NORETURN [[noreturn]]
#else
#  define UNUSED __attribute__((__unused__))
#  define NORETURN __attribute__((__noreturn__))
#endif

#ifdef _MSC_VER
#  define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#  define _CRT_SECURE_NO_WARNINGS
#endif
#if defined _WIN32 || defined _WIN64
#  define WIN32_LEAN_AND_MEAN
#  define DOSISH
#  define NOMINMAX
#elif defined __linux__ || defined __bsd__ || defined bsd ||             \
      defined __unix__  || defined unix    || __has_include(<unistd.h>) 
#  define UNIXISH
#else
#  error "What the hell kind of OS are you even using?"
#endif

#include "contrib/constructor_hax.h"

/*--------------------------------------------------------------------------------------*/
/* Actual headers */

/* I'm lazy and dumb just include all the things */
#include <algorithm>
#include <any>
#include <atomic>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <new>
#include <queue>
#include <regex>
#include <scoped_allocator>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined DOSISH
//#  include <Windows.h>
//#  include <direct.h>
//#  include <io.h>
//#  include <winsock2.h>
//#  include <afunix.h>
#elif defined UNIXISH
//#  include <dirent.h>
//#  include <fcntl.h>
//#  include <spawn.h>
//#  include <sys/socket.h>
//#  include <sys/stat.h>
//#  include <sys/wait.h>
//#  include <unistd.h>
#endif

#include <fmt/core.h>

#include "antlr4-common.h"
#include "antlr4-runtime.h"

#include "lexer/X4Lex.h"
#include "parser/X4Parse.h"
#include "parser/X4ParseBaseListener.h"
#include "parser/X4ParseBaseVisitor.h"
#include "parser/X4ParseListener.h"
#include "parser/X4ParseVisitor.h"

#endif

/****************************************************************************************/
#endif /* Common.hh */
