#include <halberd/variable_declaration.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::variable_declaration::variable_declaration(std::string name) noexcept : declaration(std::move(name))
{
}
