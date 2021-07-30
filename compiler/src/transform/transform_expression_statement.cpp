#include <halberd/transform/transform_expression_statement.h>

// halberd::syntax
#include <halberd/expression_statement.h>


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::statement> ns::transform_expression_statement::operator()(std::tuple<std::unique_ptr<syntax::expression>> value) const
{
    return std::make_unique<syntax::expression_statement>(std::get<0U>(std::move(value)));
}
