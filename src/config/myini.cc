#include "Common.hh"
#include "myini.hh"
#include "translate/script_to_xml/util/util.hh"

#include <fmt/core.h>

#define SLS(str) ("" str ""), (static_cast<size_t>(sizeof(str) - 1LLU))

namespace x4c::config
{

static void traverse(ini::X4ConfigParser::DocumentContext *tree);
static void handle_node(antlr4::RuleContext *ctx);
static void handle_statement(ini::X4ConfigParser::StatementContext *ctx);
static void handle_SectionDecl(ini::X4ConfigParser::SectionDeclContext *ctx);
static void handle_KeyValuePair(ini::X4ConfigParser::KeyValuePairContext *ctx);

void
idunnoman(std::string const &fname)
{
      antlr4::ANTLRFileStream input;
      input.loadFromFile(fname);

      auto lexer   = ini::X4ConfigLexer(&input);
      auto tokens  = antlr4::CommonTokenStream(&lexer);
      auto parser  = ini::X4ConfigParser(&tokens);
      auto *mytree = parser.document();

      traverse(mytree);

      std::cout << x4c::translate::script::util::myStringTree::to_string_tree(mytree, &parser, true)
                << "\n\n";
}

static void
traverse(ini::X4ConfigParser::DocumentContext *tree)
{
      auto f = ini::X4ConfigParser::KeyValuePairContext(tree, 0);

      std::cout << std::endl;
      for (const auto &child : tree->children) {
            if (antlrcpp::is<antlr4::RuleContext *>(child))
                  handle_node(static_cast<antlr4::RuleContext *>(child));
            else {
                  auto *tmp = tree->EOF();
                  assert(tmp == child);
            }
      }


      std::cout << std::endl;
}

static void
handle_node(antlr4::RuleContext *ctx)
{
      switch (ctx->getRuleIndex()) {
      case ini::X4ConfigParser::RuleDocument:
            break;
      case ini::X4ConfigParser::RuleStatement:
            handle_statement(static_cast<ini::X4ConfigParser::StatementContext *>(ctx));
            break;
      case ini::X4ConfigParser::RuleSectionDecl:
            handle_SectionDecl(static_cast<ini::X4ConfigParser::SectionDeclContext *>(ctx));
            break;
      case ini::X4ConfigParser::RuleKeyValuePair:
            handle_KeyValuePair(static_cast<ini::X4ConfigParser::KeyValuePairContext *>(ctx));
            break;
      default:
            throw std::runtime_error("Impossible");
      }
}

static void handle_statement(ini::X4ConfigParser::StatementContext *ctx)
{
      if (ctx->sectionDecl() != nullptr)
            handle_SectionDecl(ctx->sectionDecl());
      else if (ctx->keyValuePair() != nullptr)
            handle_KeyValuePair(ctx->keyValuePair());
}

static void handle_SectionDecl(ini::X4ConfigParser::SectionDeclContext *ctx)
{
      fmt::print("\033[1m[{}]\033[0m\n", ctx->name->getText());
}

static void handle_KeyValuePair(ini::X4ConfigParser::KeyValuePairContext *ctx)
{
      fmt::print("\"{}\" = \"{}\"\n", ctx->key->getText(), (ctx->value) ? ctx->value->getText() : "");
}

} // namespace x4c::config
