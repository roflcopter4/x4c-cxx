#
# src/translate/xml_to_script
#

add_library(translate_xml_to_script OBJECT
    nil.cc
)

add_dependencies(translate_xml_to_script x4c_gen_headers)
