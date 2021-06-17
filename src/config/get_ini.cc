#include "Common.hh"
#include "ini_wrapper.hh"
#include "config.hh"

#include <map>

namespace x4c::config
{

std::unique_ptr<map>
get_config(char const *fname)
{
      auto const ini = ini_wrapper(fname);
      return get_config(ini);
}

std::unique_ptr<map>
get_config(ini_wrapper const &ini)
{
      return get_config(&ini);
}

std::unique_ptr<map>
get_config(ini_wrapper const *const ini)
{
      auto cfg = std::make_unique<map>();
      char const *ret;

      /* MSVC insists on using double parentheses for the assignment statement.
       * Probably better not to wonder why. Therein lies madness. */
      cfg->emplace("windows", ((ret = ini->get("paths", "windows"))) ? ret : "");
      cfg->emplace("wsl",     ((ret = ini->get("paths", "wsl")))     ? ret : "");
      cfg->emplace("unix",    ((ret = ini->get("paths", "unix")))    ? ret : "");
      cfg->emplace("cygwin",  ((ret = ini->get("paths", "cygwin")))  ? ret : "");

      return cfg;
}


} // namespace x4c::config
