#ifndef X4C_TRANSLATE_AST_AST_HH_
#define X4C_TRANSLATE_AST_AST_HH_

#include "Common.hh"

#define ND [[nodiscard]]

namespace x4c::translate::AST::Junk {
/****************************************************************************************/

/*
 * Dummy base class for all actual expression nodes.
 */
class Expression
{
    protected:
      struct prec {
            enum V {
                  NIL = 0,
                  NOT,
                  UNARY_PREFIX,
                  POWER_OP,
                  MULTIPLICATIVE_OP,
                  ADDITIVE_OP,
                  COMPARITIVE_OP,
                  EQUALITY_OP,
                  AND_OP,
                  OR_OP,
                  TERNIARY_EXPRESSION,
                  OBJECT,
            };
      };
      struct who {
            enum V {
                  NIL = 0,
            };
      };

    private:
      std::string name_{};
      std::string raw_text_{};
      prec::V     prec_ = prec::NIL;

    public:
      //      Expression()                     = delete;
      //      ~Expression()                    = delete;
      //      Expression(const Expression &o)  = delete;
      //      Expression(const Expression &&o) = delete;
      //      Expression &operator=(const Expression &o) = delete;
      //      Expression &operator=(const Expression &&o) = delete;

      Expression() = default;
      explicit Expression(char const *raw, prec::V prec = prec::NIL);
      explicit Expression(std::string const &raw, prec::V prec = prec::NIL);

      ND prec::V prec() const { return prec_; }
};

/*--------------------------------------------------------------------------------------*/

namespace kinds::expr {

class UnaryPrefix
{
    private:
      static constexpr char const *const types_[] = {
            "not",
      };

      class type
      {
          private:
            std::string const repr_{};
          protected:
            // static constexpr const char repr_[] = "not";
          public:
            ND virtual std::string const &repr() const { return repr_; }
            // static std::string &repr() { return std::string(repr_); }
      };

    public:
      class NOT : public type
      {
            // static constexpr const char repr_[] = "not";
            std::string const repr_ = "not";

            public:

      };
      class PLUS;
      class MINUS;
      class ATSIGN;
      class TYPEOF;
};

namespace UnaryPrefixN {

enum V {
      NIL = 0,
      NOT,
      PLUS,
      MINUS,
      ATSIGN,
      TYPEOF,
};

} // namespace UnaryPrefixN
} // namespace kinds::expr

template <kinds::expr::UnaryPrefixN::V k>
class UnaryPrefix : public Expression
{
    private:
      kinds::expr::UnaryPrefixN::V kind = k;

      Expression *expr_ = nullptr;

    public:
      UnaryPrefix() = default;
};

#if 0
class NOT : public UnaryPrefix
{};

class UNARY_PLUS : public UnaryPrefix
{};

class UNARY_MINUS : public UnaryPrefix
{};
#endif


/****************************************************************************************/
} // namespace x4c::translate::AST::Junk
#undef ND
#endif
