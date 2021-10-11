#include <halberd/variable_declaration_statement.h>
#include <halberd/variable_declaration.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::variable_declaration_statement::variable_declaration_statement(std::unique_ptr<variable_declaration> vd) noexcept : declaration_statement(std::move(vd))
{
}

void ns::variable_declaration_statement::accept(visitor& v)
{
    v.visit(*this);
}

void ns::variable_declaration_statement::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::variable_declaration& ns::variable_declaration_statement::get_variable_declaration()
{
    return static_cast<variable_declaration&>(*_dec);
}

const ns::variable_declaration& ns::variable_declaration_statement::get_variable_declaration() const
{
    return static_cast<const variable_declaration&>(*_dec);
}
