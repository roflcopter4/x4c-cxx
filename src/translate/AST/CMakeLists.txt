#
# src/translate/AST
#

add_library(translate_AST OBJECT
    expression.cc
    what_am_I_doing.cc
)

add_dependencies(translate_AST x4c_gen_headers)
