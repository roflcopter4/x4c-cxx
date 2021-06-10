#pragma once
#include <utility>

#include "Common.hh"

#define ND [[nodiscard]]

namespace x4c::translate::AST {
/****************************************************************************************/

using std::string;

namespace util {
extern string const emptystring;
} // namespace util

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
      unsigned precedence_{};
      string   raw_{};
      groups   group_; // = GROUPS_NIL;
      types    type_;  // = TYPES_NIL;

    protected:
      void SetRaw(string const &raw) { raw_ = raw; }
      ND string const &Raw() const { return raw_; }

    public:
      explicit Expression(groups group = GROUPS_NIL, types type = TYPES_NIL);
      explicit Expression(types type);
      virtual ~Expression() = default;

      void SetGroup(groups const group) { group_ = group; }
      void SetType(types const type) { type_ = type; }

      ND groups Group() const { return group_; }
      ND types  Type() const { return type_; }

      ND virtual string const &Repr() const    = 0;
      ND virtual string const &DumpStr() const = 0;

      inline void DumpFile(std::ostream &stream = std::cout) const
      {
            stream << DumpStr() << std::endl;
      }
};

template<Expression::groups gp, Expression::types tp>
class BaseExpression : public Expression
{
    private:

    public:
      BaseExpression() : Expression(gp, tp) {}
};

class alignas(128) UnaryExpression : public Expression
{
    private:
      Expression *expr_{};

    public:
#define LAAZY(VAR, FN) ((VAR) ? ((VAR)->FN()) : util::emptystring)
      ND string const &Repr()    const override { return LAAZY(expr_, Repr); }
      ND string const &DumpStr() const override { return LAAZY(expr_, DumpStr); }
#undef LAAZY

      explicit UnaryExpression(types type) : Expression(type) {}
      explicit UnaryExpression(groups group = GROUPS_NIL, types type = TYPES_NIL)
          : Expression(group, type)
      {}

      ~UnaryExpression() override;
      /* Just use a fucking pointer */
      UnaryExpression(const UnaryExpression &o)             = delete;
      UnaryExpression(const UnaryExpression &&o)            = delete;
      UnaryExpression &operator=(const UnaryExpression &o)  = delete;
      UnaryExpression &operator=(const UnaryExpression &&o) = delete;
};


class alignas(128) BinaryExpression : public Expression
{
    private:
      Expression *left_{};
      Expression *right_{};

    public:
      ND string const &Repr()    const override { return util::emptystring; }
      ND string const &DumpStr() const override { return util::emptystring; }

      explicit BinaryExpression(types type) : Expression(type) {}
      explicit BinaryExpression(groups group = GROUPS_NIL, types type = TYPES_NIL)
          : Expression(group, type)
      {}

      ~BinaryExpression() override;
      /* Just use a fucking pointer */
      BinaryExpression(const BinaryExpression &o)             = delete;
      BinaryExpression(const BinaryExpression &&o)            = delete;
      BinaryExpression &operator=(const BinaryExpression &o)  = delete;
      BinaryExpression &operator=(const BinaryExpression &&o) = delete;
};


class alignas(128) TerniaryExpression : public Expression
{
    private:
      Expression *condition_{};
      Expression *if_true_{};
      Expression *if_false_{};

    public:
      ND string const &Repr()    const override { return util::emptystring; }
      ND string const &DumpStr() const override { return util::emptystring; }

      explicit TerniaryExpression(types type) : Expression(type) {}
      explicit TerniaryExpression(groups group = GROUPS_NIL, types type = TYPES_NIL)
          : Expression(group, type)
      {}

      ~TerniaryExpression() override;
      /* Just use a fucking pointer */
      TerniaryExpression(const TerniaryExpression &o)             = delete;
      TerniaryExpression(const TerniaryExpression &&o)            = delete;
      TerniaryExpression &operator=(const TerniaryExpression &o)  = delete;
      TerniaryExpression &operator=(const TerniaryExpression &&o) = delete;
};


namespace junk {
extern void ass(Expression * expr);
} // namespace junk

/****************************************************************************************/
} // namespace x4c::translate::AST
#undef ND
