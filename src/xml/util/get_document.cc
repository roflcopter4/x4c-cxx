#include "Common.hh"
#include <exception>
#include <stdexcept>

#include "xml/util/util.hh"

namespace x4c::XML::util {

xmlDoc *
get_document(const char *fname)
{
        xmlDoc *ret = xmlReadFile(fname, "UTF-8", 0);
        if (!ret)
                throw std::runtime_error("Parsing failed");
        return ret;
}

xmlTextReader *
get_reader(xmlDoc *doc)
{
        xmlTextReader *ret = xmlReaderWalker(doc);
        if (!ret)
                throw std::runtime_error("Failed to create xml walker");
        return ret;
}

} // namespace x4c::XML::util::intern
