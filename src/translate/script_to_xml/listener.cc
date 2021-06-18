#include "Common.hh"
#include "listener.hh"

namespace x4c::translate::script {

using x4c::gen::parser::X4Parse;

void
ParseListener::enterExpression(X4Parse::ExpressionContext *ctx)
// ParseListener::exitExpression(X4Parse::ExpressionContext *ctx)
{

}

void
// ParseListener::enterExpression(X4Parse::ExpressionContext *ctx)
ParseListener::exitExpression(X4Parse::ExpressionContext *ctx)
{
      if (auto neg = ctx->negationOp(); !neg.empty()) {
            for (auto const &unary : neg) {
                  // auto *expr = new AST::UnaryExpression(AST::Expression::TYPES_UNARY_PREFIX_NEGATION, ch);
                  auto *expr = new AST::UnaryExpression(AST::Expression::TYPES_UNARY_PREFIX_OP, unary->getText());
                              // parse->getTokenNames().at(unary->getRuleIndex()));
                              // parse->getVocabulary().getSymbolicName(unary->getRuleIndex()));
                  *want[0] = current = expr;
                  want[0]  = expr->expr_ptr();
            }
      }
      if (auto *unary = ctx->unaryOp(); unary != nullptr) {
            auto *expr = new AST::UnaryExpression(AST::Expression::TYPES_UNARY_PREFIX_OP, unary->getText());
                                                      // parse->getTokenNames().at(unary->getRuleIndex()));
                                                  // parse->getVocabulary().getSymbolicName(unary->getRuleIndex()));
            *want[0] = current = expr;
            want[0]  = expr->expr_ptr();
      }
}

ParseListener::ParseListener(X4Parse *p)
{
      parse = p;
      top = parent = current = nullptr;
      want[0] = &top;
      want[1] = nullptr;
      want[2] = nullptr;
}

ParseListener::~ParseListener()
{
      delete top;
}

/*--------------------------------------------------------------------------------------*/

std::unique_ptr<AST::Expression>
ParseListener::payload()
{
      std::unique_ptr<AST::Expression> ret = nullptr;
      if (top) {
            ret = std::unique_ptr<AST::Expression>(top);
            top = nullptr;
      }
      return ret;
}

} // namespace x4c::translate::script
