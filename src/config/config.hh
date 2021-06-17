#pragma once

#include "Common.hh"
#include <map>

#include "ini_wrapper.hh"

namespace x4c::config
{

using map = std::map<std::string const, std::string const>;

extern std::unique_ptr<map> get_config(char const *fname);
extern std::unique_ptr<map> get_config(ini_wrapper const &ini);
extern std::unique_ptr<map> get_config(ini_wrapper const *ini);

} // namespace x4c::config
