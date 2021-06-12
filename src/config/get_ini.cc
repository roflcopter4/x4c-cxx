#include "Common.hh"
#include "ini_wrapper.hh"

#include <map>

namespace x4c::config
{

std::unique_ptr<std::map<string, string>>
get_config(char const *fname)
{
      auto cfg = std::make_unique<std::map<string, string>>();
      auto ini = ini_wrapper(fname);

      cfg->emplace("windows", ini.get("paths", "windows") ?: "");
      cfg->emplace("wsl",     ini.get("paths", "wsl") ?: "");
      cfg->emplace("unix",    ini.get("paths", "unix") ?: "");
      cfg->emplace("cygwin",  ini.get("paths", "cygwin") ?: "");

      return cfg;
}

} // namespace x4c::config
