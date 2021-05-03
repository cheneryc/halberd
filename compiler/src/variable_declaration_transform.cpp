#include <halberd/variable_declaration_transform.h>


namespace
{
    namespace ns = halberd::compiler;
}

std::unique_ptr<halberd::syntax::variable_declaration> ns::variable_declaration_transform::operator()(std::tuple<lexer::keyword, std::string> value) const
{
    lexer::keyword keyword;
    std::string identifier;

    std::tie(keyword, identifier) = std::move(value);

    return std::make_unique<syntax::variable_declaration>(std::move(identifier));
}
