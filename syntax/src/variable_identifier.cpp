#include <halberd/variable_identifier.h>
#include <halberd/variable_declaration.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>

#include <stdexcept> // std::invalid_argument


namespace
{
    namespace ns = halberd::syntax;
}

ns::variable_identifier::variable_identifier(variable_declaration* dec) : _declaration(dec)
{
    if (!_declaration)
    {
        throw std::invalid_argument("halberd::syntax::variable_identifier: variable_declaration argument cannot be null");
    }
}

void ns::variable_identifier::accept(visitor& v)
{
    v.visit(*this);
}

void ns::variable_identifier::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::type ns::variable_identifier::get_type() const
{
    return get_variable_declaration().get_type();
}

const ns::variable_declaration& ns::variable_identifier::get_variable_declaration() const noexcept
{
    return *_declaration;
}
