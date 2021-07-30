#include <halberd/declaration.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::declaration::declaration(std::string name) noexcept : _name(std::move(name))
{
}

ns::declaration::~declaration() = default;

const char* ns::declaration::get_name() const noexcept
{
    return _name.c_str();
}
