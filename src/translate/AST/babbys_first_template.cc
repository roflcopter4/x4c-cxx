// ReSharper disable CppInconsistentNaming

#include "Common.hh"
//#include "translate/AST/babbys_first_template.hh"
#include "translate/AST/what_am_I_doing.hh"

#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace x4c::translate::AST::stupid
{

static Expression *
retard()
{
      auto *bin = new UnaryExpression(Expression::TYPES_UNARY_PREFIX_NEGATION);
      return bin;
}

void
helpme()
{
      auto *dunno = retard();

      try {
            std::cout << "Type: " << typeid(dunno).name() << " --- " << typeid(*dunno).name() << '\n';
            std::cout << static_cast<void *>(dynamic_cast<UnaryExpression *>(dunno)->expr()) << std::endl;
      } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
      }

      delete dunno;
}

} // namespace x4c::translate::AST::stupid
