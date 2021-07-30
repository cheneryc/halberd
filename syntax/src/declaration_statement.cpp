#include <halberd/declaration_statement.h>
#include <halberd/declaration.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::declaration_statement::declaration_statement(std::unique_ptr<declaration> dec) noexcept : _dec(std::move(dec))
{
}
