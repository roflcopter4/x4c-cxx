#
# src/config
#

add_library(x4c_config OBJECT
      ini_wrapper.cc
      myini.cc
      get_ini.cc
)

add_dependencies(x4c_config x4c_gen_headers)

