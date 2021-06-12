#
# src/translate
#

add_subdirectory(xml_to_script)
add_subdirectory(script_to_xml)
add_subdirectory(AST)

# add_library(x4c_translate STATIC
#     $<TARGET_OBJECTS:translate_xml_to_script>
#     $<TARGET_OBJECTS:translate_script_to_xml>
#     $<TARGET_OBJECTS:translate_AST>
# )

# target_precompile_headers(x4c_translate REUSE_FROM x4c)

# add_dependencies(x4c_translate x4c_gen_headers)
