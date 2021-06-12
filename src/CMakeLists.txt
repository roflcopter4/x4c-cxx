# /src

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib64")

add_definitions(-D_BUILD_)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
set (x4c_SOURCES
    main.cc
    junk/thingmain.cc
    junk/num_grouping.c

    translate/AST/expression.cc
    translate/AST/what_am_I_doing.cc

    translate/script_to_xml/util/string_tree.cc

    xml/doc_wrapper.cc
    xml/util/get_document.cc
)

set (x4c_headers
 
)

add_executable(x4c
    $<TARGET_OBJECTS:x4c_parser>
    ${x4c_SOURCES}
)

#target_compile_options(x4c
#    PRIVATE
#    ${ALL_THE_CXX_FLAGS}
#    # $<JOIN:${ALL_THE_CXX_FLAGS},"">
#)

# target_precompile_headers(x4c
#     PRIVATE
#     $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/_common.hh>
# )


add_subdirectory(Gen)
# add_subdirectory(translate)
# add_subdirectory(xml)
# add_subdirectory(config)

add_dependencies(x4c x4c_gen_headers)
target_precompile_headers(x4c
    # REUSE_FROM $<$<COMPILE_LANGUAGE:CXX>:x4c_parser>
    REUSE_FROM x4c_parser
)

################################################################################
# Libraries

target_link_libraries(x4c
    # -static
    # -Wl,-Bdynamic
    # -lfmt
    # -liniparser4
    # -lxml2
    # -Wl,-Bstatic
    ${CMAKE_THREAD_LIBS_INIT}
    ${IniParser_LIBRARIES}
    fmt::fmt
    -lxml2
    # /usr/local/lib64/libantlr4-runtime.a
    # -Wl,-Bstatic
    # -Wl,--no-whole-archive
    -lantlr4-runtime
    # -Wl,-Bdynamic
    # /usr/lib/gcc/x86_64-pc-linux-gnu/11.1.0/libstdc++.a
    # /usr/lib/gcc/x86_64-pc-linux-gnu/11.1.0/libstdc++.a
)

if (MINGW)
    # For some reason clang wants libstdc++ linked in on Windows.
    target_link_libraries(x4c -lstdc++)
endif ()

if (USE_JEMALLOC STREQUAL "YES")
    target_link_libraries(x4c ${JEMALLOC_LIBRARY})
endif()
