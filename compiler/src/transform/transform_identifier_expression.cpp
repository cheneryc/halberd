#include <halberd/transform/transform_identifier_expression.h>

// halberd::syntax
#include <halberd/identifier_expression.h>


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::expression> ns::transform_identifier_expression::operator()(std::tuple<std::string> value) const
{
    return std::make_unique<syntax::identifier_expression>(std::get<0U>(std::move(value)));
}
