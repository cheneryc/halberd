#include <halberd/transform/transform_variable_declaration.h>


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::variable_declaration> ns::transform_variable_declaration::operator()(std::tuple<lexer::keyword, std::string> value) const
{
    lexer::keyword keyword;
    std::string identifier;

    std::tie(keyword, identifier) = std::move(value);

    return std::make_unique<syntax::variable_declaration>(std::move(identifier));
}
