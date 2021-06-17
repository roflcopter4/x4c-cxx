#ifndef CONFIG_WRAPPER_HH_
#define CONFIG_WRAPPER_HH_

#include "Common.hh"
#define ND [[nodiscard]]

namespace x4c::config
{
#include <cstdio>
#include <map>

#include <iniparser4/iniparser.h>

using std::string;

class ini_wrapper
{
    private:
      string filename_;
      dictionary *c_dict_;

      //std::unique_ptr<std::map<string const, string const>> map_;

    public:
      explicit ini_wrapper(string const &filename);
      explicit ini_wrapper(char const *filename);
      ~ini_wrapper();

      ini_wrapper(const ini_wrapper &o)            = delete;
      ini_wrapper &operator=(const ini_wrapper &o) = delete;
      ini_wrapper(ini_wrapper &&o)                 = default;
      ini_wrapper &operator=(ini_wrapper &&o)      = default;

      ND string const & filename() const { return filename_; }

      void dbg_dump(char const *fname) const;
      void dbg_dump(FILE *fp = stderr) const;

      char const *get(char const *section, char const *key) const;
      char const *get(char const *key) const;

      void set(char const *section, char const *key, char const *value) const;
      void set(char const *key, char const *value) const;

      void unset(char const *section, char const *key) const;
      void unset(char const *key) const;

      void write() const { write(filename()); }
      void write(string const &fname) const;
};

} // namespace x4c::config

#undef ND
#endif /* ini_wrapper.hh */
