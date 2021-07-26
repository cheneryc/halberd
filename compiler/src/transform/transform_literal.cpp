#include <halberd/transform/transform_literal.h>


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::literal> ns::transform_literal::operator()(std::tuple<int> value) const
{
    return std::make_unique<halberd::syntax::literal>(std::get<0U>(value));
}

std::unique_ptr<halberd::syntax::literal> ns::transform_literal::operator()(std::tuple<float> value) const
{
    return std::make_unique<halberd::syntax::literal>(std::get<0U>(value));
}
