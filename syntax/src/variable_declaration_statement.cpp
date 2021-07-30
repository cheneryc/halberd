#include <halberd/variable_declaration_statement.h>
#include <halberd/variable_declaration.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::variable_declaration_statement::variable_declaration_statement(std::unique_ptr<variable_declaration> vd) noexcept : declaration_statement(std::move(vd))
{
}
