#include <halberd/variable_declaration.h>
#include <halberd/visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::variable_declaration::variable_declaration(std::string name) noexcept : declaration(std::move(name))
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
