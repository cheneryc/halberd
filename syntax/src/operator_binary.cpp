#include <halberd/operator_binary.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::operator_binary::operator_binary(operator_binary_id operator_id, std::unique_ptr<expression> operand_lhs, std::unique_ptr<expression> operand_rhs) : operator_id(operator_id),
    _operand_lhs(std::move(operand_lhs)),
    _operand_rhs(std::move(operand_rhs))
{
}

const ns::expression& ns::operator_binary::get_operand_lhs() const
{
    return *_operand_lhs;
}

const ns::expression& ns::operator_binary::get_operand_rhs() const
{
    return *_operand_rhs;
}
