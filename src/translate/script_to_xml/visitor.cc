#include "Common.hh"
#include "translate/script_to_xml/util/util.hh"
#include "translate/script_to_xml/visitor.hh"

namespace x4c::translate::script {
/*======================================================================================*/

ParseVisitor::ParseVisitor(X4Parse *p)
{
      parse = p;
      top = parent = current = nullptr;
      want = &top;
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

/*--------------------------------------------------------------------------------------*/

antlrcpp::Any
ParseVisitor::visitExpression(X4Parse::ExpressionContext *ctx)
{
      if (auto neg = ctx->negationOp(); !neg.empty()) {
            for (UNUSED auto const &ch : neg) {
                  auto *expr = new AST::UnaryExpression(AST::Expression::TYPES_UNARY_PREFIX_OP, "!");
                  *want = current = expr;
                  want  = expr->expr_ptr();
            }
      }
      if (auto *unary = ctx->unaryOp(); unary != nullptr) {
            auto *expr = new AST::UnaryExpression(AST::Expression::TYPES_UNARY_PREFIX_OP, unary->getText());
            *want = current = expr;
            want  = expr->expr_ptr();
      }

      return visitChildren(ctx);
}

/*--------------------------------------------------------------------------------------*/
/* Subexpressions */

template <typename Ty>
antlrcpp::Any
ParseVisitor::visit_subexpression(Ty *ctx,
                                  AST::Expression::types const type,
                                  char const *const real_op)
{
      auto *expr = new AST::BinaryExpression(type, (real_op ? real_op : ctx->Op->getText()));
      *want = current = expr;

      expr->SetRaw(ctx->getText());

      want = expr->left_ptr();
      visit(ctx->Left);
      want = expr->right_ptr();
      visit(ctx->Right);

      return false;
}

antlrcpp::Any
ParseVisitor::visitSubexprPower(X4Parse::SubexprPowerContext *ctx)
{
      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_POWER);
}

antlrcpp::Any
ParseVisitor::visitSubexprMultiplicative(X4Parse::SubexprMultiplicativeContext *ctx)
{
      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_MULTIPLICATION);
}

antlrcpp::Any
ParseVisitor::visitSubexprAdditive(X4Parse::SubexprAdditiveContext *ctx)
{
      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_ADDITION);
}

antlrcpp::Any
ParseVisitor::visitSubexprComparitive(X4Parse::SubexprComparitiveContext *ctx)
{
      char const *real_op{};

      /* Nobody likes `gt` or `le` nonsense. */
      if (ctx->Op->groupGE())
            real_op = ">=";
      else if (ctx->Op->groupLE())
            real_op = "<=";
      else if (ctx->Op->groupGT())
            real_op = ">";
      else if (ctx->Op->groupLT())
            real_op = "<";
      else
            throw std::runtime_error("Impossible token?!?");

      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_COMPARISON, real_op);
}

antlrcpp::Any
ParseVisitor::visitSubexprEquality(X4Parse::SubexprEqualityContext *ctx)
{
      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_EQUALITY);
}

antlrcpp::Any
ParseVisitor::visitSubexprAnd(X4Parse::SubexprAndContext *ctx)
{
      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_AND, "&&");
}

antlrcpp::Any
ParseVisitor::visitSubexprOr(X4Parse::SubexprOrContext *ctx)
{
      return visit_subexpression(ctx, AST::Expression::TYPES_BINARY_OR, "||");
}

antlrcpp::Any
ParseVisitor::visitSubexprTerniary(X4Parse::SubexprTerniaryContext *ctx)
{
      auto *expr = new AST::TerniaryExpression(ctx->Third ? AST::Expression::TYPES_TERNIARY_THREE_TERM
                                                          : AST::Expression::TYPES_TERNIARY_TWO_TERM);
      *want = current = expr;

      expr->SetRaw(ctx->getText());

      want = expr->condition_ptr();
      visit(ctx->First);
      want = expr->if_true_ptr();
      visit(ctx->Second);
      if (ctx->Third) {
            want = expr->if_false_ptr();
            visit(ctx->Third);
      }

      return false;
}

/*======================================================================================*/
} // namespace x4c::translate::script
