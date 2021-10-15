#include <halberd/transform/transform_variable_declaration.h>

// halberd::syntax
#include <halberd/type.h>

#include <stdexcept> // std::invalid_argument


namespace
{
    namespace ns = halberd::compiler::transform;

    halberd::syntax::type to_type(halberd::lexer::keyword keyword)
    {
        halberd::syntax::type_id type_id;

        switch (keyword)
        {
            case halberd::lexer::keyword::strict_i8:
                type_id = halberd::syntax::type_id::type_i8;
                break;
            case halberd::lexer::keyword::strict_i16:
                type_id = halberd::syntax::type_id::type_i16;
                break;
            case halberd::lexer::keyword::strict_i32:
                type_id = halberd::syntax::type_id::type_i32;
                break;
            case halberd::lexer::keyword::strict_i64:
                type_id = halberd::syntax::type_id::type_i64;
                break;
            default:
                throw std::invalid_argument("invalid keyword for halberd::compiler::transform::transform_variable_declaration");
        }

        return { type_id };
    }
}

std::unique_ptr<halberd::syntax::variable_declaration> ns::transform_variable_declaration::operator()(std::tuple<lexer::keyword, std::string> value) const
{
    lexer::keyword keyword;
    std::string identifier;

    std::tie(keyword, identifier) = std::move(value);

    return std::make_unique<syntax::variable_declaration>(to_type(keyword), std::move(identifier));
}
