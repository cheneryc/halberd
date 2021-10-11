#include <halberd/operator_binary.h>
#include <halberd/visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::operator_binary::operator_binary(operator_binary_id operator_id, std::unique_ptr<expression> operand_lhs, std::unique_ptr<expression> operand_rhs) : operator_id(operator_id),
    _operand_lhs(std::move(operand_lhs)),
    _operand_rhs(std::move(operand_rhs))
{
}

void ns::operator_binary::accept(visitor& v)
{
    v.visit(*this);
}

void ns::operator_binary::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::expression& ns::operator_binary::get_operand_lhs()
{
    return *_operand_lhs;
}

const ns::expression& ns::operator_binary::get_operand_lhs() const
{
    return *_operand_lhs;
}

ns::expression& ns::operator_binary::get_operand_rhs()
{
    return *_operand_rhs;
}

const ns::expression& ns::operator_binary::get_operand_rhs() const
{
    return *_operand_rhs;
}
