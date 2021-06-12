#include "Common.hh"

#include "what_am_I_doing.hh"

namespace x4c::translate::AST::util {
string const emptystring{};
} // namespace x4c::translate::AST::util

/****************************************************************************************/
namespace x4c::translate::AST {

Expression::Expression(Expression::groups group, Expression::types type)
{
      group_ = group;
      type_  = type;
}

Expression::Expression(Expression::types const type)
{
      switch ((type_ = type)) {
      case TYPES_UNARY_PREFIX_NEGATION:
      case TYPES_UNARY_PREFIX_OP:
            group_ = GROUPS_UNARY_PREFIX;
            break;
      case TYPES_UNARY_POSTFIX_OP:
      case TYPES_UNARY_POSTFIX_CAST:
            group_ = GROUPS_UNARY_POSTFIX;
            break;
      case TYPES_BINARY_POWER:
      case TYPES_BINARY_MULTIPLICATION:
      case TYPES_BINARY_ADDITION:
      case TYPES_BINARY_COMPARISON:
      case TYPES_BINARY_EQUALITY:
      case TYPES_BINARY_AND:
      case TYPES_BINARY_OR:
            group_ = GROUPS_BINARY;
            break;
      case TYPES_TERNIARY_TWO_TERM:
      case TYPES_TERNIARY_THREE_TERM:
            group_ = GROUPS_TERNIARY;
            break;
      case TYPES_NIL:
            group_ = GROUPS_NIL;
            break;
      }
}

/*--------------------------------------------------------------------------------------*/

UnaryExpression::~UnaryExpression()
{
      delete expr_;
}

BinaryExpression::~BinaryExpression()
{
      delete left_;
      delete right_;
}

TerniaryExpression::~TerniaryExpression()
{
      delete condition_;
      delete if_true_;
      delete if_false_;
}

/*--------------------------------------------------------------------------------------*/



/*======================================================================================*/
namespace junk {
void
ass(Expression *expr)
{
      auto *foo = reinterpret_cast<BinaryExpression *>(expr);
      std::cout << foo->Repr() << expr->Type() << std::endl;
}
} // namespace junk

} // namespace x4c::translate::AST
/****************************************************************************************/
