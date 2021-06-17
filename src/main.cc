// ReSharper disable CppInconsistentNaming
#include "Common.hh"

#include "translate/AST/what_am_I_doing.hh"
#include "translate/script_to_xml/listener.hh"
#include "translate/script_to_xml/util/util.hh"
#include "translate/script_to_xml/visitor.hh"
#include <cstddef>

using antlr4::CommonTokenStream, antlr4::ANTLRInputStream;
using lexer::X4Lex, parser::X4Parse;
using x4c::translate::script::ParseVisitor;

namespace script = x4c::translate::script;
namespace AST    = x4c::translate::AST;

namespace cani {

struct parse_wrapper {
      std::string raw_input;
      ANTLRInputStream  *stream;
      CommonTokenStream *tokens;
      X4Lex             *lex;
      X4Parse           *parse;
      //ParseVisitor      visitor;
      //AST::Expression   *payload;
      union {
            X4Parse::DebugStatementContext *dbg;
            X4Parse::DocumentContext       *doc;
      } tree{};

      explicit parse_wrapper(std::string &&in) {
            stream = new ANTLRInputStream(in);
            lex    = new X4Lex(stream);
            tokens = new CommonTokenStream(lex);
            parse  = new X4Parse(tokens);
            raw_input = std::move(in);
      }
      explicit parse_wrapper(char const *in) : parse_wrapper(std::string(in)) {}
      explicit parse_wrapper(std::istream &instr)
          : parse_wrapper(std::string(std::istreambuf_iterator<char>{instr}, {})) {}

      ~parse_wrapper() {
            delete stream;
            delete tokens;
            delete lex;
            delete parse;
      }
};

static void do_something(char const *arg);
static void do_something_else(char const *arg);
} // namespace cani

/*--------------------------------------------------------------------------------------*/

namespace x4c::translate::AST::stupid { extern void helpme(); }

int
main(int argc, char *argv[])
{
#if 0
      x4c::junk::thing::main(argc--, argv++);

      if (argc > 1) {
            auto ini = x4c::config::ini_wrapper(argv[1]);
            ini.dbg_dump();
      }

      const auto foo = x4c::config::get_config(argv[1]);
      for (const auto &[first, second] : *foo)
            fmt::print("'{}'  =>  '{}'\n", first, second);

      auto x = ::AST::expression_impl<::AST::Binary>(
          ::AST::expression_impl<::AST::Binary>::TYPES_BINARY_POWER);
#endif

      auto *x = new AST::BinaryExpression(AST::Expression::TYPES_UNARY_POSTFIX_OP);
      AST::junk::ass(x);
      delete x;

      if (argc > 1)
            cani::do_something_else(argv[1]);
#if 0
            cani::do_something(argv[1]);
#endif

      // AST::stupid::helpme();

      return 0;
}

namespace cani {
static void
do_something(char const *const arg)
{
      auto p = std::make_unique<parse_wrapper>(arg);
      auto *tree = p->parse->debugStatement();
      fmt::print("{}\n", script::util::to_string_tree(tree, p->parse, true));

      auto visit = script::ParseVisitor(p->parse);
      visit.visitDebugStatement(tree);

      auto pl = visit.payload();
      
      // std::cout << "Typeid: " << typeid(pl).name() << "\nBase typeid: " << typeid(pl.get()).name() << std::endl;

      auto *foo = dynamic_cast<AST::UnaryExpression *>(pl.release());
      fmt::print("Op: {}\nExprOp: {}\n", foo->op(), foo->expr()->op());
      delete foo;
}

static void
do_something_else(char const *arg)
{
      std::ifstream in(arg);
      auto p = std::make_unique<parse_wrapper>(in);
      in.close();

      std::cout << p->raw_input << std::endl;

      auto *tree = p->parse->document();
      std::cout << tree << std::endl;
}

} // namespace cani
