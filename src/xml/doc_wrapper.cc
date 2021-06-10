#include "Common.hh"
#include <cstring>
#include <exception>
#include <filesystem>
#include <libgen.h>
#include <stdexcept>

#include "xml/doc_wrapper.hh"
#include "xml/util/util.hh"

namespace x4c::XML {

doc_wrapper::doc_wrapper(char const *const file)
        : fname_(file) 
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
                throw std::runtime_error(std::string("No such file or directory: ") + file);

        char *tmp = strdup(file);
        base_     = basename(tmp);
        free(tmp);

        tmp   = realpath(file, nullptr);
        full_ = tmp;
        free(tmp);

        tmp   = strdup(full_.c_str());
        path_ = dirname(tmp);
        free(tmp);
}

} // namespace x4c::XML
