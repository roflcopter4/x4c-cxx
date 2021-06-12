#ifndef X4C_TRANSLATE_AST_EXPRESSION_HH_
#define X4C_TRANSLATE_AST_EXPRESSION_HH_

#include "Common.hh"

#define ND [[nodiscard]]

namespace x4c::translate::AST::fuckme {
/****************************************************************************************/

using std::string;

template <typename ty>
class expression_impl;

using Expression = expression_impl<int>;

template <typename ty>
class expression_impl
{
    public:
      enum groups {
            EXPR_NIL = 0,
            EXPR_UNARY_PREFIX,
            EXPR_BINARY,
            EXPR_TERNIARY,
      };
      // enum unary_types {
      enum types {
            TYPES_NIL = 0,
            TYPESUNARY_NEGATION,
            TYPESUNARY_OTHER,
            // PLUS,
            // MINUS,
            // ATSIGN,
            // TYPEOF,
            // };

            // enum binary_types {
            // TYPES_BINARY_NIL = 0,
            TYPES_BINARY_POWER,
            TYPES_BINARY_MULTIPLICATION,
            TYPES_BINARY_ADDITION,
            TYPES_BINARY_COMPARISON,
            TYPES_BINARY_EQUALITY,
            TYPES_BINARY_AND,
            TYPES_BINARY_OR,
            // };

            // enum terniary_types {
            // TYPES_TERNIARY_NIL,
            TYPES_TERNIARY_TWO_TERM,
            TYPES_TERNIARY_THREE_TERM,
      };

    private:
      string      operator_;
      enum groups group_ = EXPR_NIL;
      enum types  type_  = TYPES_NIL;
      ty data_;

      // ty          type_{};

    public:
      explicit expression_impl(enum types val) { type_ = val; }
      ND ty &get() const { return data_; }
};


class UnaryPrefix //: expression_impl<enum unary_types>
{
    public:
      // UnaryPrefix(unary_types type, expression_impl *expr) : expression_impl(type)
      // {
      //       // expr_ = expr;
      // }

    private:
      // expression_impl *expr_ = new expression_impl<int>(0);

      class Negation
      {};
      class Other
      {};

      union {
            Negation negation_{};
            Other    other_;
      };
};

class Binary //: public expression_impl<enum binary_types>
{
    public:
    private:
};

class Terniary //: public expression_impl<enum terniary_types>
{
    public:
    private:
};

/****************************************************************************************/
} // namespace x4c::translate::AST::fuckme
#undef ND
#endif
