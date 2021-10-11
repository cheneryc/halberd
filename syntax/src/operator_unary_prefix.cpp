#include <halberd/operator_unary_prefix.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::operator_unary_prefix::operator_unary_prefix(operator_unary_prefix_id operator_id, std::unique_ptr<expression> operand) : operator_id(operator_id), _operand(std::move(operand))
{
}

void ns::operator_unary_prefix::accept(visitor& v)
{
    v.visit(*this);
}

void ns::operator_unary_prefix::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::expression& ns::operator_unary_prefix::get_operand()
{
    return *_operand;
}

const ns::expression& ns::operator_unary_prefix::get_operand() const
{
    return *_operand;
}
