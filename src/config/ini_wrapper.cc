#include "Common.hh"

#include "ini_wrapper.hh"

namespace x4c::config
{
/*--------------------------------------------------------------------------------------*/

ini_wrapper::ini_wrapper(char const *const filename) : ini_wrapper(string(filename))
{}

ini_wrapper::ini_wrapper(string const &filename)
{
      filename_ = filename;
      c_dict_   = iniparser_load(filename_.c_str());
      if (!c_dict_)
            throw std::runtime_error("Failed to parse file \"" + filename_ + '"');


}

ini_wrapper::~ini_wrapper()
{
      iniparser_freedict(c_dict_);
}

/*--------------------------------------------------------------------------------------*/

void
ini_wrapper::dbg_dump(char const *fname)
{
      FILE *fp = fopen(fname, "rb");
      assert(fp != nullptr);
      dbg_dump(fp);
      fclose(fp);
}

void
ini_wrapper::dbg_dump(FILE *fp)
{
      iniparser_dump(c_dict_, fp);
}

void
ini_wrapper::write(string const &fname)
{
      FILE *fp = fopen(fname.c_str(), "wb");
      assert(fp != nullptr);
      iniparser_dump_ini(c_dict_, fp);
      fclose(fp);
}

/*--------------------------------------------------------------------------------------*/

static char *
combine_section_and_key(char const *section, char const *key)
{
      string tmp;
      char       *ret = nullptr;

      if (section && key)
            tmp = string(section) + ':' + string(key);
      else if (section)
            tmp = section;
      else if (key)
            tmp = key;

      if (!tmp.empty())
            ret = strdup(tmp.c_str());
      return ret;
}

char const *
ini_wrapper::get(char const *section, char const *key)
{
      char       *tmp = combine_section_and_key(section, key);
      char const *ret = get(tmp);
      free(static_cast<void *>(tmp));
      return ret;
}

char const *
ini_wrapper::get(char const *key)
{
      return dictionary_get(c_dict_, key, nullptr);
}


void
ini_wrapper::set(char const *section, char const *key, char const *value)
{
      char *tmp = combine_section_and_key(section, key);
      set(tmp, value);
      free(static_cast<void *>(tmp));
}

void
ini_wrapper::set(char const *key, char const *value)
{
      int ret = dictionary_set(c_dict_, key, value);
      if (ret)
            throw std::runtime_error("Failed to set ini key:value pair.");
}


void
ini_wrapper::unset(char const *section, char const *key)
{
      char *tmp = combine_section_and_key(section, key);
      unset(tmp);
      free(static_cast<void *>(tmp));
}

void 
ini_wrapper::unset(char const *key)
{
      dictionary_unset(c_dict_, key);
}

/*--------------------------------------------------------------------------------------*/
} // namespace x4c::config
