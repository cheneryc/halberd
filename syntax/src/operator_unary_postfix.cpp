#include <halberd/operator_unary_postfix.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::operator_unary_postfix::operator_unary_postfix(operator_unary_postfix_id operator_id, std::unique_ptr<expression> operand) : operator_id(operator_id), _operand(std::move(operand))
{
}
