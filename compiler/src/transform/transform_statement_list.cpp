#include <halberd/transform/transform_statement_list.h>


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::statement_list> ns::transform_statement_list::operator()(std::tuple<std::vector<std::unique_ptr<syntax::statement>>> value) const
{
    return std::make_unique<syntax::statement_list>(std::get<0U>(std::move(value)));
}
