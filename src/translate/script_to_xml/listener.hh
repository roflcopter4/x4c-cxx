// ReSharper disable CppInconsistentNaming
#pragma once

#include "Common.hh"
#include "translate/AST/what_am_I_doing.hh"
#define ND [[nodiscard]]

namespace x4c::translate::script {

class listener_data
{

};

class ParseListener: public gen::parser::X4ParseBaseListener
{
    private:
      AST::Expression *top;
      AST::Expression *parent;
      AST::Expression *current;
      gen::parser::X4Parse *parse;

      std::stack<bool> pop_current{};

    protected:
      AST::Expression **want[3]{};

    public:
      explicit ParseListener(gen::parser::X4Parse *p);
      ~ParseListener() override;

      ParseListener(const ParseListener &o)            = delete;
      ParseListener(ParseListener &&o)                 = delete;
      ParseListener &operator=(const ParseListener &o) = delete;
      ParseListener &operator=(ParseListener &&o)      = delete;

      ND std::unique_ptr<AST::Expression> payload();

      void enterExpression(gen::parser::X4Parse::ExpressionContext *ctx) override;
      void exitExpression(gen::parser::X4Parse::ExpressionContext *ctx) override;
};

} // namespace x4c::translate::script
#undef ND
