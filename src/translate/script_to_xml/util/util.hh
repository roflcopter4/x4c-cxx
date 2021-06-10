#ifndef X4C_TRANSLATE_SCRIPT_TO_XML_UTIL_UTIL_HH_
#define X4C_TRANSLATE_SCRIPT_TO_XML_UTIL_UTIL_HH_

#include "Common.hh"
/*--------------------------------------------------------------------------------------*/

namespace x4c::translate::script::util::myStringTree {

using PT = antlr4::tree::ParseTree;
using sVec = std::vector<std::string>;
using antlr4::Parser;

extern std::string to_string_tree(PT *t, bool pretty = false);
extern std::string to_string_tree(PT *t, Parser *recog, bool pretty = false);
extern std::string to_string_tree(PT *t, sVec const &rule_names, bool pretty = false);

extern std::string recursive_method(PT *t, bool pretty = false);
extern std::string recursive_method(PT *t, Parser *recog, bool pretty = false);
extern std::string recursive_method(PT *t, sVec const &rule_names, bool pretty = false);

} // namespace x4c::translate::script::util::myStringTree


#endif /* translate/script_to_xml/util/util.hh */
