#
# src/Gen
#

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_definitions(-D_BUILD_)

set (x4c_gen_parser_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4Parse.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseBaseListener.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseBaseVisitor.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseListener.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseVisitor.cpp"
)

set (x4c_gen_parser_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4Parse.h"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseBaseListener.h"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseBaseVisitor.h"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseListener.h"
    "${CMAKE_CURRENT_BINARY_DIR}/parser/X4ParseVisitor.h"
)

set (x4c_gen_lexer_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/lexer/X4Lex.cpp")
set (x4c_gen_lexer_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/lexer/X4Lex.h")

set (x4c_gen_ini_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/ini/X4ConfigParser.cpp"
                         "${CMAKE_CURRENT_BINARY_DIR}/ini/X4ConfigLexer.cpp")
set (x4c_gen_ini_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/ini/X4ConfigParser.h"
                         "${CMAKE_CURRENT_BINARY_DIR}/ini/X4ConfigLexer.h")

##########################################################################################

macro (ADD_ANTLR4_COMMAND _packageName _antlrSource _visitor _listener _outputSRC _outputH)
    add_custom_command(
        OUTPUT ${${_outputSRC}} ${${_outputH}}
        COMMAND antlr4 -Xexact-output-dir -Dlanguage=Cpp -long-messages
                -package "${_packageName}"
                $<IF:$<BOOL:${_visitor}>,-visitor,-no-visitor>
                $<IF:$<BOOL:${_listener}>,-listener,-no-listener>
                -o "${CMAKE_CURRENT_BINARY_DIR}/${_packageName}"
                "${CMAKE_CURRENT_SOURCE_DIR}/${_antlrSource}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_antlrSource}"
        VERBATIM
    )
endmacro()

#add_custom_command(
#    OUTPUT ${x4c_gen_parser_SOURCES} ${x4c_gen_parser_HEADERS}
#    COMMAND antlr4
#            -Xexact-output-dir -Dlanguage=Cpp -package parser
#            -listener -visitor -long-messages -o "${CMAKE_CURRENT_BINARY_DIR}/parser"
#            "${CMAKE_CURRENT_SOURCE_DIR}/X4Parse.g4"
#    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/X4Parse.g4"
#    VERBATIM
#)
# 
#add_custom_command(
#    OUTPUT ${x4c_gen_lexer_SOURCES} ${x4c_gen_lexer_HEADERS}
#    COMMAND antlr4
#            -Xexact-output-dir -Dlanguage=Cpp
#            -package lexer -long-messages -o "${CMAKE_CURRENT_BINARY_DIR}/lexer"
#            "${CMAKE_CURRENT_SOURCE_DIR}/X4Lex.g4"
#    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/X4Lex.g4"
#    VERBATIM
#)

ADD_ANTLR4_COMMAND(lexer X4Lex.g4 FALSE FALSE x4c_gen_lexer_SOURCES x4c_gen_lexer_HEADERS)
ADD_ANTLR4_COMMAND(parser X4Parse.g4 TRUE TRUE x4c_gen_parser_SOURCES x4c_gen_parser_HEADERS)
# ADD_ANTLR4_COMMAND(ini X4Config.g4 FALSE FALSE x4c_gen_ini_SOURCES x4c_gen_ini_HEADERS)

##########################################################################################

add_library(x4c_gen_headers INTERFACE
    ${x4c_gen_parser_HEADERS}
    ${x4c_gen_lexer_HEADERS}
    # ${x4c_gen_ini_HEADERS}
)

# add_library(x4c_gen_parser OBJECT ${x4c_gen_parser_SOURCES})
# add_dependencies(x4c_gen_parser x4c_gen_headers)
# 
# add_library(x4c_gen_lexer OBJECT ${x4c_gen_lexer_SOURCES})
# add_dependencies(x4c_gen_lexer x4c_gen_headers)
# 
# add_library(x4c_parser SHARED)
# target_link_libraries(x4c_parser
#     x4c_gen_lexer
#     x4c_gen_parser
# )


add_library(x4c_parser OBJECT
    ${x4c_gen_parser_SOURCES}
    ${x4c_gen_lexer_SOURCES}
    # ${x4c_gen_ini_SOURCES}
    nothing.c
)

target_precompile_headers(x4c_parser
    PRIVATE
    "../_common.hh"
    # $<$<COMPILE_LANGUAGE:CXX>:"_common.hh">
    # $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_BINARY_DIR}/parser/X4Parse.h>
    # $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_BINARY_DIR}/lexer/X4Lex.h>
)

add_dependencies(x4c_parser x4c_gen_headers)

# target_compile_options(x4c_parser PRIVATE
#     -fPIC -flto
# )
