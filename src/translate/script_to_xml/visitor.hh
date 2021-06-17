// ReSharper disable CppInconsistentNaming
#pragma once

#include "Common.hh"
#include "translate/AST/what_am_I_doing.hh"
#define ND [[nodiscard]]

namespace x4c::translate::script {

class ParseVisitor: public parser::X4ParseBaseVisitor
{
    private:
      AST::Expression *top;
      AST::Expression *parent;
      AST::Expression *current;
      parser::X4Parse *parse;

      std::stack<bool> pop_current{};

    protected:
      AST::Expression **want[3]{};

    public:
      explicit ParseVisitor(parser::X4Parse *p);
      ~ParseVisitor() override;

      ParseVisitor(const ParseVisitor &o)            = delete;
      ParseVisitor(ParseVisitor &&o)                 = delete;
      ParseVisitor &operator=(const ParseVisitor &o) = delete;
      ParseVisitor &operator=(ParseVisitor &&o)      = delete;

      ND std::unique_ptr<AST::Expression> payload();

      antlrcpp::Any visitExpression(parser::X4Parse::ExpressionContext *ctx) override;
};

} // namespace x4c::translate::script
#undef ND
