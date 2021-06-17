#include "Common.hh"
#include "translate/script_to_xml/util/util.hh"

namespace x4c::translate::script::util {

using namespace antlr4;
using namespace tree;
using std::string, std::vector, std::stack, std::endl;

static string get_node_text(ParseTree *t, vector<string> const &rule_names);
static string get_node_text(ParseTree *t);
static string make_indent(int depth);

constexpr static char one_indent[3] = "  ";

/*--------------------------------------------------------------------------------------*/

#ifdef _WIN32
static bool add_colour = true;

HAX_INITIALIZER(watch_out_for_cmd_exe)
{
      auto *term = getenv("TERM");
      add_colour = (term && strcmp(term, "mintty") == 0);
}
#else
static constexpr bool add_colour = true;
#endif

static std::string
escape_hitespace(std::string const &str, bool const escape_spaces)
{
      std::string result;
      for (auto c : str) {
            switch (c) {
            case '\n':
                  result += "\\n";
                  break;

            case '\r':
                  result += "\\r";
                  break;

            case '\t':
                  result += "\\t";
                  break;

            case ' ':
                  if (escape_spaces) {
                        result +=  "\u00B7";
                        break;
                  }
                  // FALLTHROUGH
            default:
                  result += c;
            }
      }

      return result;
}

/*--------------------------------------------------------------------------------------*/

string
to_string_tree(ParseTree *t, bool const pretty)
{
      return to_string_tree(t, nullptr, pretty);
}

string
to_string_tree(ParseTree *t, Parser *recog, bool const pretty)
{
      if (recog == nullptr)
            return to_string_tree(t, vector<string>(), pretty);
      return to_string_tree(t, recog->getRuleNames(), pretty);
}

string
to_string_tree(ParseTree *t, vector<string> const &rule_names, bool const pretty)
{
      auto temp = escape_hitespace(get_node_text(t, rule_names), false);
      if (t->children.empty())
            return temp;

      std::stringstream ss;
      ss << '(' << temp << ' ';

      // Implement the recursive walk as iteration to avoid trouble with deep nesting.
      stack<size_t> ind_stack;
      ParseTree *run               = t;
      size_t     child_index       = 0;
      size_t     indentation_level = 0;

      while (child_index < run->children.size()) {
            if (child_index > 0)
                  ss << ' ';

            ParseTree *child = run->children[child_index];
            temp = escape_hitespace(get_node_text(child, rule_names), false);

            if (!child->children.empty()) {
                  // Go one level deeper.
                  ind_stack.push(child_index);
                  run         = child;
                  child_index = 0;

                  if (pretty) {
                        ++indentation_level;
                        ss << std::endl;
                        for (size_t i = 0; i < indentation_level; ++i)
                              ss << one_indent;
                  }

                  ss << '(' << temp << ' ';
            } else {
                  ss << temp;
                  while (++child_index == run->children.size()) {
                        if (!ind_stack.empty()) {
                              // Reached the end of the current level.
                              // See if we can step up from here.
                              child_index = ind_stack.top();
                              ind_stack.pop();
                              run = run->parent;

                              if (pretty)
                                    --indentation_level;

                              ss << ')';
                        } else {
                              break;
                        }
                  }
            }
      }

      ss << ')';
      return ss.str();
}

/*--------------------------------------------------------------------------------------*/

static string recursive_method_impl(ParseTree *t, vector<string> const *rule_names, bool pretty);
static string recurse(int *depth, ParseTree *node, vector<string> const *rule_names, bool pretty);

string recursive_method(ParseTree *t, vector<string> const &rule_names, bool const pretty)
{
      return recursive_method_impl(t, &rule_names, pretty);
}


string recursive_method(ParseTree *t, bool const pretty)
{
      return recursive_method_impl(t, nullptr, pretty);
}

string recursive_method(ParseTree *t, Parser *recog, bool const pretty)
{
      return recursive_method_impl(t, &recog->getRuleNames(), pretty);
}

static string
recursive_method_impl(ParseTree *t, vector<string> const *const rule_names, bool const pretty)
{
      int depth = 0;
      return recurse(&depth, t, rule_names, pretty);
}

static string
recurse(int *depth, ParseTree *node, vector<string> const *rule_names, bool const pretty)
{
      auto str = (rule_names)
            ? escape_hitespace(get_node_text(node, *rule_names), false)
            : escape_hitespace(get_node_text(node), false);

      if (node->children.empty())
            return str;

      string ret;

      if (pretty) {
            if (*depth > 0)
                  ret += '\n' + make_indent(*depth);
            ++*depth;
      }
      ret += '(' + str + ' ';

      if (!node->children.empty()) {
            ret += recurse(depth, node->children[0], rule_names, pretty);
            for (unsigned i = 1; i < node->children.size(); ++i)
                  ret += ' ' + recurse(depth, node->children[i], rule_names, pretty);
      }

      if (pretty)
            --*depth;

      ret += ')';
      return ret;
}

/*--------------------------------------------------------------------------------------*/
/* Some utils */

static string
get_node_text(ParseTree *t, vector<string> const &rule_names)
{
      if (antlrcpp::is<RuleContext *>(t)) {
            string ret;

            if (!rule_names.empty()) {
                  size_t const alt_number = dynamic_cast<RuleContext *>(t)->getAltNumber();
                  size_t const rule_index = dynamic_cast<RuleContext *>(t)->getRuleIndex();
                  string const rule_name  = rule_names[rule_index];

                  /* This is very unlikely. */
                  if (alt_number != atn::ATN::INVALID_ALT_NUMBER)
                        return rule_name + ":" + std::to_string(alt_number);

                  ret = rule_name;
            } else {
                  ret = dynamic_cast<RuleContext *>(t)->getText();
            }

            if (add_colour)
                  return "\033[0;36m" + ret + "\033[0m";
            return ret;
      }

      if (antlrcpp::is<ErrorNode *>(t)) {
            if (add_colour)
                  return "\033[1;31m" + t->toString() + "\033[0m";
            return t->toString();
      }

      if (antlrcpp::is<TerminalNode *>(t)) {
            if (Token *symbol = dynamic_cast<TerminalNode *>(t)->getSymbol(); symbol) {
                  if (add_colour)
                        return "\033[1;35m" + symbol->getText() + "\033[0m";
                  return symbol->getText();
            }
      }

      return "";
}

static string
get_node_text(ParseTree *t)
{
      // no recog for rule names
      if (antlrcpp::is<RuleContext *>(t))
            return dynamic_cast<RuleContext *>(t)->getText();
      if (antlrcpp::is<TerminalNodeImpl *>(t))
            return dynamic_cast<TerminalNodeImpl *>(t)->getSymbol()->getText();
      if (antlrcpp::is<ErrorNode *>(t))
            return t->toString();

      return "";
}

static __inline string
make_indent(int depth)
{
      string ret;
      for (; depth > 0; --depth)
            ret += one_indent;
      return ret;
}

} // namespace x4c::translate::script::util
