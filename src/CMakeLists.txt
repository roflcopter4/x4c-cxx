# /src

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib64")

add_definitions(-D_BUILD_)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Gen)
add_subdirectory(translate)
add_subdirectory(xml)
add_subdirectory(config)

set (x4c_SOURCES
    main.cc
    junk/thingmain.cc
    junk/num_grouping.c
)

set (x4c_headers
 
)

add_executable(x4c ${x4c_SOURCES})

add_dependencies(x4c x4c_gen_headers)

################################################################################
# Libraries

target_link_libraries(x4c
    x4c_parser
    x4c_xml
    x4c_config
    x4c_translate
    ${CMAKE_THREAD_LIBS_INIT}
    ${IniParser_LIBRARIES}
    fmt::fmt
    -lantlr4-runtime
    -lxml2
)

if (MINGW)
    # For some reason clang wants libstdc++ linked in on Windows.
    target_link_libraries(x4c -lstdc++)
endif ()

if (USE_JEMALLOC STREQUAL "YES")
    target_link_libraries(x4c ${JEMALLOC_LIBRARY})
endif()
