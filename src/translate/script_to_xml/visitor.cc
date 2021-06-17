#include "Common.hh"
#include "translate/script_to_xml/util/util.hh"
#include "translate/script_to_xml/visitor.hh"

namespace x4c::translate::script {

using namespace parser;

// ParseListener::enterExpression(X4Parse::ExpressionContext *ctx)
antlrcpp::Any
ParseVisitor::visitExpression(parser::X4Parse::ExpressionContext *ctx)
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

      fmt::print("GOT:\t{}\n{}\n\n", ctx->getText(), util::to_string_tree(ctx, parse, true));
      return visitChildren(ctx);
}

ParseVisitor::ParseVisitor(X4Parse *p)
{
      parse = p;
      top = parent = current = nullptr;
      want[0] = &top;
      want[1] = nullptr;
      want[2] = nullptr;
}

ParseVisitor::~ParseVisitor()
{
      delete top;
}

/*--------------------------------------------------------------------------------------*/

std::unique_ptr<AST::Expression>
ParseVisitor::payload()
{
      std::unique_ptr<AST::Expression> ret = nullptr;
      if (top) {
            ret = std::unique_ptr<AST::Expression>(top);
            top = nullptr;
      }
      return ret;
}

} // namespace x4c::translate::script
