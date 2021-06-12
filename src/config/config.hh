#pragma once

#include "Common.hh"
#include <map>

namespace x4c::config
{

using map = std::unique_ptr<std::map<std::string, std::string>>;

extern map get_config(char const *fname);

} // namespace x4c::config
