#include <halberd/operator_unary_postfix.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::operator_unary_postfix::operator_unary_postfix(operator_unary_postfix_id operator_id, std::unique_ptr<expression> operand) : operator_id(operator_id), _operand(std::move(operand))
{
}

void ns::operator_unary_postfix::accept(visitor& v)
{
    v.visit(*this);
}

void ns::operator_unary_postfix::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

void ns::operator_unary_postfix::reset_operand(std::unique_ptr<expression> exp)
{
    _operand = std::move(exp);
}

ns::expression& ns::operator_unary_postfix::get_operand()
{
    return *_operand;
}

const ns::expression& ns::operator_unary_postfix::get_operand() const
{
    return *_operand;
}
