#include <halberd/variable_declaration.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::variable_declaration::variable_declaration(type t, std::string name) noexcept : declaration(std::move(name)), _type(t)
{
}

void ns::variable_declaration::accept(visitor& v)
{
    v.visit(*this);
}

void ns::variable_declaration::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::type ns::variable_declaration::get_type() const
{
    return _type;
}
