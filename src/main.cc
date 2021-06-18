// ReSharper disable CppInconsistentNaming
#include "Common.hh"

#include "translate/AST/what_am_I_doing.hh"
#include "translate/script_to_xml/listener.hh"
#include "translate/script_to_xml/util/util.hh"
#include "translate/script_to_xml/visitor.hh"

using antlr4::CommonTokenStream, antlr4::ANTLRInputStream;
using x4c::gen::lexer::X4Lex, x4c::gen::parser::X4Parse;
using x4c::translate::script::ParseVisitor;

namespace script = x4c::translate::script;
namespace AST    = x4c::translate::AST;

namespace cani {

#define ND [[nodiscard]]

class alignas(64) parse_wrapper
{
    private:
      std::string        raw_input_;
      ANTLRInputStream  *stream_;
      CommonTokenStream *tokens_;
      X4Lex             *lex_;
      X4Parse           *parse_;

    public:
      explicit parse_wrapper(std::string &&in) {
            raw_input_ = std::move(in);
            stream_    = new ANTLRInputStream(raw_input_);
            lex_       = new X4Lex(stream_);
            tokens_    = new CommonTokenStream(lex_);
            parse_     = new X4Parse(tokens_);
      }
      explicit parse_wrapper(char const *in) : parse_wrapper(std::string(in)) {}
      explicit parse_wrapper(std::istream &instr)
          : parse_wrapper(std::string(std::istreambuf_iterator<char>{instr}, {})) {}

      ~parse_wrapper() {
            delete stream_;
            delete tokens_;
            delete lex_;
            delete parse_;
      }

      parse_wrapper(const parse_wrapper &o)            = delete;
      parse_wrapper(parse_wrapper &&o)                 = delete;
      parse_wrapper &operator=(const parse_wrapper &o) = delete;
      parse_wrapper &operator=(parse_wrapper &&o)      = delete;

      ND std::string const &raw()    const { return raw_input_; }
      ND ANTLRInputStream  *stream() const { return stream_; }
      ND CommonTokenStream *tokens() const { return tokens_; }
      ND X4Lex             *lex()    const { return lex_; }
      ND X4Parse           *parse()  const { return parse_; }
};

#undef ND

static void do_something(char const *arg);
static void do_something_else(char const *arg);
} // namespace cani

/*--------------------------------------------------------------------------------------*/

namespace x4c::translate::AST::stupid {
extern void helpme();
} // namespace x4c::translate::AST::stupid

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

      if (argc > 1)
            cani::do_something(argv[1]);
            //cani::do_something_else(argv[1]);

      // AST::stupid::helpme();

      return 0;
}

namespace cani {
UNUSED static void
do_something(char const *const arg)
{
      auto *p     = new parse_wrapper(arg);
      auto *tree  = p->parse()->debugStatement();
      auto *visit = new script::ParseVisitor(p->parse());

      fmt::print("{}\n", script::util::to_string_tree(tree, p->parse(), true));
      visit->visitDebugStatement(tree);

      auto *pl = visit->payload().release();
      std::cout << pl->Raw() << '\n';

      delete p;
      delete visit;

      // auto *pl = dynamic_cast<AST::UnaryExpression *>(visit.payload().release());
      // fmt::print("Op: {}\nExprOp: {}\n", pl->op(), pl->expr()->op());
      // delete pl;
}

static void
do_something_else(char const *arg)
{
      auto in = std::ifstream(arg);
      auto *p = new parse_wrapper(in);
      in.close();

      std::cout << p->raw() << '\n';
      auto *tree = p->parse()->document();
      std::cout << tree << '\n';
}

} // namespace cani
