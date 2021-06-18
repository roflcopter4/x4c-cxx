// ReSharper disable CppInconsistentNaming
#ifndef X4C_TRANSLATE_SCRIPT_VISITOR_HH_
#define X4C_TRANSLATE_SCRIPT_VISITOR_HH_

#include "Common.hh"
#include "translate/AST/what_am_I_doing.hh"
#define ND [[nodiscard]]

namespace x4c::translate::script {

using gen::parser::X4Parse;

class ParseVisitor: public gen::parser::X4ParseBaseVisitor
{
    private:
      AST::Expression *top;
      AST::Expression *parent;
      AST::Expression *current;
      gen::parser::X4Parse *parse;

      std::stack<bool> pop_current{};

      template <typename Ty>
      antlrcpp::Any visit_subexpression(Ty *ctx,
                                        AST::Expression::types type,
                                        char const *real_op = nullptr);

      AST::Expression **want{};

    public:
      explicit ParseVisitor(gen::parser::X4Parse *p);
      ~ParseVisitor() override;

      ParseVisitor(const ParseVisitor &o)            = delete;
      ParseVisitor(ParseVisitor &&o)                 = delete;
      ParseVisitor &operator=(const ParseVisitor &o) = delete;
      ParseVisitor &operator=(ParseVisitor &&o)      = delete;

      ND std::unique_ptr<AST::Expression> payload();

      antlrcpp::Any visitExpression           (X4Parse::ExpressionContext            *ctx) override;
      antlrcpp::Any visitSubexprPower         (X4Parse::SubexprPowerContext          *ctx) override;
      antlrcpp::Any visitSubexprMultiplicative(X4Parse::SubexprMultiplicativeContext *ctx) override;
      antlrcpp::Any visitSubexprAdditive      (X4Parse::SubexprAdditiveContext       *ctx) override;
      antlrcpp::Any visitSubexprComparitive   (X4Parse::SubexprComparitiveContext    *ctx) override;
      antlrcpp::Any visitSubexprEquality      (X4Parse::SubexprEqualityContext       *ctx) override;
      antlrcpp::Any visitSubexprAnd           (X4Parse::SubexprAndContext            *ctx) override;
      antlrcpp::Any visitSubexprOr            (X4Parse::SubexprOrContext             *ctx) override;
      antlrcpp::Any visitSubexprTerniary      (X4Parse::SubexprTerniaryContext       *ctx) override;
};

} // namespace x4c::translate::script
#undef ND
#endif
