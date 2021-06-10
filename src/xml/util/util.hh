#ifndef X4C_XML_UTIL_UTIL_HH_
#define X4C_XML_UTIL_UTIL_HH_

#include "Common.hh"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>

#include "xml/doc_wrapper.hh"


namespace x4c::XML::util {

extern xmlDoc        *get_document(const char *fname);
extern xmlTextReader *get_reader(xmlDoc *doc);

} // namespace x4c::XML::util


#endif
