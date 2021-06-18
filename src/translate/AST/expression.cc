#include "Common.hh"

#include "translate/AST/AST.hh"

namespace x4c::translate::AST::Junk {
/****************************************************************************************/


Expression::Expression(char const *raw, prec::V const prec)
    : Expression(std::string(raw), prec)
{}
Expression::Expression(std::string const &raw, prec::V const prec)
{
      raw_text_ = raw;
      prec_     = prec;
}

static void foo()
{
      auto x = kinds::expr::UnaryPrefix::NOT();
      const auto &y = x.repr();
}

/****************************************************************************************/
} // namespace x4c::translate::AST::Junk
