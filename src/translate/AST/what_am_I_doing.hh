// ReSharper disable CppInconsistentNaming
#ifndef X4C_TRANSLATE_AST_WHATAMIDOING_HH_
#define X4C_TRANSLATE_AST_WHATAMIDOING_HH_

#include "Common.hh"

namespace x4c::translate::script {
class ParseVisitor;
} // namespace x4c::translate::script

#define ND [[nodiscard]]

namespace x4c::translate::AST {
/****************************************************************************************/


using std::string;

namespace util {
extern string const emptystring;
} // namespace util

/*--------------------------------------------------------------------------------------*/

/**
 * \brief I dunno like whatever or something I guess.
 */
class Expression
{
    public:
      enum groups {
            GROUPS_NIL = 0,
            GROUPS_UNARY_PREFIX,
            GROUPS_UNARY_POSTFIX,
            GROUPS_BINARY,
            GROUPS_TERNIARY,
            GROUPS_OBJECT,
      };
      enum types {
            TYPES_NIL = 0,

            TYPES_UNARY_PREFIX_NEGATION,
            TYPES_UNARY_PREFIX_OP,

            TYPES_UNARY_POSTFIX_OP,
            TYPES_UNARY_POSTFIX_CAST,

            TYPES_BINARY_POWER,
            TYPES_BINARY_MULTIPLICATION,
            TYPES_BINARY_ADDITION,
            TYPES_BINARY_COMPARISON,
            TYPES_BINARY_EQUALITY,
            TYPES_BINARY_AND,
            TYPES_BINARY_OR,

            TYPES_TERNIARY_TWO_TERM,
            TYPES_TERNIARY_THREE_TERM,
      };

    private:
      string op_;
      string raw_{};
      groups group_; // = GROUPS_NIL;
      types  type_;  // = TYPES_NIL;

    protected:

    public:
      explicit Expression(types type = TYPES_NIL, string const &op = "");
      virtual ~Expression() = default;

      Expression(Expression const &o)            = delete;
      Expression(Expression &&o)                 = delete;
      Expression &operator=(Expression const &o) = delete;
      Expression &operator=(Expression &&o)      = delete;

      void SetRaw(string const &raw) { raw_ = raw; }
      ND string const &Raw() const { return raw_; }
      void SetGroup(groups const group) { group_ = group; }
      void SetType(types const type) { type_ = type; }

      ND groups Group() const { return group_; }
      ND types  Type() const { return type_; }

      ND virtual string const &Repr() const    = 0;
      ND virtual string const &DumpStr() const = 0;
      ND string const &op() const { return op_; }

      void DumpFile(std::ostream &stream = std::cout) const
      {
            stream << DumpStr() << std::endl;
      }
};

/*--------------------------------------------------------------------------------------*/

#define BOILERPLATE_EXPR(NAME)                              \
      void set_##NAME(Expression *expr) { NAME##_ = expr; } \
      ND Expression *NAME() const { return NAME##_; }       \
      ND Expression **NAME##_ptr() { return &NAME##_; }

template <auto foo> constexpr void set_thingy(Expression *expr) { foo = expr; }

class UnaryExpression : public Expression
{
    private:
      Expression *expr_{};

    public:
#define LAAZY(VAR, FN) ((VAR) ? ((VAR)->FN()) : util::emptystring)
      ND string const &Repr()    const override { return LAAZY(expr_, Repr); }
      ND string const &DumpStr() const override { return LAAZY(expr_, DumpStr); }
#undef LAAZY

      explicit UnaryExpression(types const type, string const &op = "") : Expression(type, op) {}
      ~UnaryExpression() override;

      /* Just use a fucking pointer */
      UnaryExpression(const UnaryExpression &o)             = delete;
      UnaryExpression(UnaryExpression &&o)                  = delete;
      UnaryExpression &operator=(const UnaryExpression &o)  = delete;
      UnaryExpression &operator=(UnaryExpression &&o)       = delete;

      BOILERPLATE_EXPR(expr);
      //void set_expr(Expression *expr) { expr_ = expr; }
      //ND Expression *expr() const { return expr_; }
      //ND Expression **expr_ptr() { return &expr_; }
};

/*--------------------------------------------------------------------------------------*/

class BinaryExpression : public Expression
{
    private:
      Expression *left_{};
      Expression *right_{};

    protected:
    public:
      ND string const &Repr()    const override { return util::emptystring; }
      ND string const &DumpStr() const override { return util::emptystring; }

      explicit BinaryExpression(types const type, string const &op = "") : Expression(type, op) {}
      ~BinaryExpression() override;

      /* Just use a fucking pointer */
      BinaryExpression(const BinaryExpression &o)             = delete;
      BinaryExpression(BinaryExpression &&o)                  = delete;
      BinaryExpression &operator=(const BinaryExpression &o)  = delete;
      BinaryExpression &operator=(BinaryExpression &&o)       = delete;

      //void set_left(Expression *expr) { left_ = expr; }
      //ND Expression *left() const { return left_; }
      //ND Expression **left_ptr() { return &left_; }

      //void set_right(Expression *expr) { right_ = expr; }
      //ND Expression *right() const { return right_; }
      //ND Expression **right_ptr() { return &right_; }

      BOILERPLATE_EXPR(left);
      BOILERPLATE_EXPR(right);
};

/*--------------------------------------------------------------------------------------*/

class TerniaryExpression : public Expression
{
    private:
      Expression *condition_{};
      Expression *if_true_{};
      Expression *if_false_{};

    public:
      ND string const &Repr()    const override { return util::emptystring; }
      ND string const &DumpStr() const override { return util::emptystring; }

      explicit TerniaryExpression(types const type) : Expression(type) {}

      ~TerniaryExpression() override;

      /* Just use a fucking pointer */
      TerniaryExpression(TerniaryExpression const &o)            = delete;
      TerniaryExpression(TerniaryExpression &&o)                 = delete;
      TerniaryExpression &operator=(TerniaryExpression const &o) = delete;
      TerniaryExpression &operator=(TerniaryExpression &&o)      = delete;

      BOILERPLATE_EXPR(condition);
      BOILERPLATE_EXPR(if_true);
      BOILERPLATE_EXPR(if_false);
};

/*--------------------------------------------------------------------------------------*/

namespace junk {
extern void ass(Expression *expr);
} // namespace junk


/****************************************************************************************/
} // namespace x4c::translate::AST
#undef ND
#endif
