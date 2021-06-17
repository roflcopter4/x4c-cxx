#include "Common.hh"
#include <filesystem>
#include <stdexcept>

#include "xml/doc_wrapper.hh"
#include "xml/util/util.hh"

namespace x4c::XML
{

doc_wrapper::doc_wrapper(char const *const file) : fname_(file)
{
      doc_ = util::get_document(fname_.full().c_str());
      rd_  = util::get_reader(doc_);
}

doc_wrapper::~doc_wrapper()
{
      xmlFreeTextReader(rd_);
      xmlFreeDoc(doc_);
}

doc_wrapper::filename::filename(char const *const file)
{
      if (!std::filesystem::exists(file))
            throw std::runtime_error(fmt::format("No such file or directory '{}'", file));

      auto const tmp = std::filesystem::canonical(file);
      full_ = tmp.string();
      base_ = tmp.filename().string();
      path_ = tmp.parent_path().string();
}

} // namespace x4c::XML
