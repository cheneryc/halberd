#include <halberd/declaration.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::declaration::declaration(std::string name) noexcept : node(), _name(std::move(name))
{
}

const char* ns::declaration::get_name() const noexcept
{
    return _name.c_str();
}
