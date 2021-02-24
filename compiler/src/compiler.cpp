#include <halberd/compiler.h>

// halberd::lexer
#include <halberd/lexer.h>
#include <halberd/scanner.h>

// halberd::parser
#include <halberd/combinator_choice.h>
#include <halberd/combinator_end.h>
#include <halberd/combinator_filter.h>
#include <halberd/combinator_sequence.h>
#include <halberd/combinator_operators.h>

// halberd::util
#include <halberd/util/type.h>

#include <iterator> // std::make_move_iterator


namespace
{
    namespace ns = halberd::compiler;

    //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
    bool is_identifier(const halberd::lexer::token* token) noexcept
    {
        return halberd::util::is_type_ptr<halberd::lexer::token_identifier>(token);
    }

    constexpr auto match_identifier_v = halberd::parser::make_filter(is_identifier);

    //TODO: make constexpr - dynamic_cast is not allowed in constexpr, so need a different approach here
    template<halberd::lexer::keyword K>
    bool is_keyword(const halberd::lexer::token* token) noexcept
    {
        auto token_identifier_reserved = dynamic_cast<const halberd::lexer::token_identifier_reserved*>(token);

        return token_identifier_reserved &&
               token_identifier_reserved->_keyword == K;
    }

    template<halberd::lexer::keyword K>
    constexpr auto match_keyword_v = halberd::parser::make_filter(is_keyword<K>);

    constexpr auto make_parser()
    {
        using namespace halberd;

        /*
            Possible tokens are denoted by all caps and include:

            KEYWORD    - lexer::token_identifier_reserved
            IDENTIFIER - lexer::token_identifier
        */

        constexpr auto parser_end = parser::combinator_end_v;

        /*
            <type_integer> ::= KEYWORD("i8")  |
                               KEYWORD("i16") |
                               KEYWORD("i32") |
                               KEYWORD("i64")
        */

        constexpr auto parser_type_integer =
            match_keyword_v<lexer::keyword::strict_i8>  |
            match_keyword_v<lexer::keyword::strict_i16> |
            match_keyword_v<lexer::keyword::strict_i32> |
            match_keyword_v<lexer::keyword::strict_i64>;

        /*
            <variable_declaration> ::= KEYWORD["var"] <type_integer> IDENTIFIER
        */

        constexpr auto parser_variable_declaration =
            match_keyword_v<lexer::keyword::strict_var> +
            parser_type_integer +
            match_identifier_v +
            parser_end;

        return parser_variable_declaration;
    }
}

bool ns::compile(const char* src)
{
    halberd::lexer::scanner scanner(halberd::lexer::get_smv_union(), src);

    auto token_source = halberd::parser::make_source(
        [&scanner]()
    {
        return scanner.scan();
    },
        [](const std::unique_ptr<halberd::lexer::token>& token_ptr)
    {
        return token_ptr.get();
    });

    return make_parser().apply(token_source).operator bool();
}

bool ns::compile(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
{
    auto token_source = halberd::parser::make_source(
        [it     = std::make_move_iterator(tokens.begin()),
         it_end = std::make_move_iterator(tokens.end())]
        () mutable
    {
        return (it != it_end) ? *(it++) : nullptr;
    },
        [](const std::unique_ptr<halberd::lexer::token>& token_ptr)
    {
        return token_ptr.get();
    });

    return make_parser().apply(token_source).operator bool();
}
