#include <halberd/operator_unary_prefix.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::operator_unary_prefix::operator_unary_prefix(operator_unary_prefix_id operator_id, std::unique_ptr<expression> operand) : operator_id(operator_id), _operand(std::move(operand))
{
}

const ns::expression& ns::operator_unary_prefix::get_operand() const
{
    return *_operand;
}
