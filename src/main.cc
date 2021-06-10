#include "Common.hh"
#include <cmath>
#include <err.h>

#include "config/config.hh"
#include "config/ini_wrapper.hh"
#include "config/myini.hh"
#include "junk/things.hh"

#include "translate/AST/expression.hh"
#include "translate/AST/what_am_I_doing.hh"

// using antlr4::CommonTokenStream, antlr4::ANTLRInputStream, antlr4::tree::ParseTree;
// using lexer::X4Lex, parser::X4Parse;

using namespace x4c::translate;

/*--------------------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
#if 0
      x4c::junk::thing::main(argc--, argv++);

      if (argc > 1) {
            auto ini = x4c::config::ini_wrapper(argv[1]);
            ini.dbg_dump();
      }

      x4c::config::idunnoman(std::string(argv[1]));

      auto foo = x4c::config::get_config(argv[1]);
      for (const auto &s : *foo)
            fmt::print("'{}'  =>  '{}'\n", s.first, s.second);
#endif

#if 0
      auto x = ::AST::expression_impl<::AST::Binary>(
          ::AST::expression_impl<::AST::Binary>::TYPES_BINARY_POWER);
#endif

      auto *x = new AST::BinaryExpression(AST::Expression::TYPES_UNARY_POSTFIX_OP);
      AST::junk::ass(x);
      delete x;

      return 0;
}
