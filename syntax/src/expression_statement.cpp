#include <halberd/expression_statement.h>
#include <halberd/expression.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::expression_statement::expression_statement(std::unique_ptr<expression> exp) noexcept : _exp(std::move(exp))
{
}

void ns::expression_statement::accept(visitor& v)
{
    v.visit(*this);
}

void ns::expression_statement::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

void ns::expression_statement::reset_expression(std::unique_ptr<expression> exp)
{
    _exp = std::move(exp);
}

ns::expression& ns::expression_statement::get_expression()
{
    return *_exp;
}

const ns::expression& ns::expression_statement::get_expression() const
{
    return *_exp;
}
