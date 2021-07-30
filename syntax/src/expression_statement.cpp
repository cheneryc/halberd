#include <halberd/expression_statement.h>
#include <halberd/expression.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::expression_statement::expression_statement(std::unique_ptr<expression> exp) noexcept : _exp(std::move(exp))
{
}
