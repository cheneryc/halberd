#include <halberd/identifier_expression.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::identifier_expression::identifier_expression(std::string id) noexcept : _id(std::move(id))
{
}

void ns::identifier_expression::accept(visitor& v)
{
    v.visit(*this);
}

void ns::identifier_expression::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::type ns::identifier_expression::get_type() const
{
    throw std::exception();
}

const char* ns::identifier_expression::get_identifier() const noexcept
{
    return _id.c_str();
}
