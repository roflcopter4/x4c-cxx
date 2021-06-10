#
# src/xml
#

add_library(x4c_xml OBJECT
    doc_wrapper.cc
    util/get_document.cc
)

add_dependencies(x4c_xml x4c_gen_headers)

# target_compile_options(
#     x4c_xml 
#     PRIVATE
#     "-I${CMAKE_CURRENT_SOURCE_DIR}"
# )
