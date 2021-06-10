# Try to find libiniparser
# Once done, this will define
#
# IniParser_FOUND        - system has libev
# IniParser_INCLUDE_DIRS - libev include directories
# IniParser_LIBRARIES    - libraries needed to use libev


#if(IniParser_INCLUDE_DIRS AND IniParser_LIBRARIES)
#    set(IniParser_FIND_QUIETLY TRUE)
#else()

set (IniParser_FOUND FALSE)

find_path(
    IniParser_INCLUDE_DIRS
    NAMES iniparser4/iniparser.h
    HINTS ${IniParser_ROOT_DIR}
    PATH_SUFFIXES include)

find_library(
    IniParser_LIBRARIES
    NAME iniparser4
    HINTS ${IniParser_ROOT_DIR}
    PATH_SUFFIXES ${CMAKE_INSTALL_LIBDIR})

if (IniParser_LIBRARIES)
    set(IniParser_FOUND TRUE CACHE INTERNAL BOOL "Found LibIniParser")
else()
    set(IniParser_FOUND FALSE CACHE INTERNAL BOOL "Failed to find LibIniParser")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    IniParser DEFAULT_MSG IniParser_LIBRARIES IniParser_INCLUDE_DIRS IniParser_FOUND)

mark_as_advanced(IniParser_LIBRARIES IniParser_INCLUDE_DIRS IniParser_FOUND)

#endif()
#
