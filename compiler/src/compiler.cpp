#include <halberd/compiler.h>

// halberd::lexer
#include <halberd/lexer.h>
#include <halberd/symbol.h>
#include <halberd/scanner.h>

// halberd::parser
#include <halberd/combinator_end.h>
#include <halberd/combinator_filter.h>
#include <halberd/combinator_function.h>
#include <halberd/combinator_select.h>
#include <halberd/combinator_transform.h>
#include <halberd/combinator_operators.h>
#include <halberd/source.h>

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

    //TODO: make constexpr - dynamic_cast is not allowed in constexpr, so need a different approach here
    template<halberd::lexer::symbol S>
    bool is_symbol(const halberd::lexer::token* token) noexcept
    {
        auto token_symbol = dynamic_cast<const halberd::lexer::token_symbol*>(token);

        return token_symbol &&
               token_symbol->_symbol == S;
    }

    template<halberd::lexer::symbol S>
    constexpr auto match_symbol_v = halberd::parser::make_filter(is_symbol<S>);

    //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
    bool is_literal_fractional(const halberd::lexer::token* token) noexcept
    {
        return halberd::util::is_type_ptr<halberd::lexer::token_literal_fractional>(token);
    }

    constexpr auto match_literal_fractional_v = halberd::parser::make_filter(is_literal_fractional);

    //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
    bool is_literal_integer(const halberd::lexer::token* token) noexcept
    {
        return halberd::util::is_type_ptr<halberd::lexer::token_literal_integer>(token);
    }

    constexpr auto match_literal_integer_v = halberd::parser::make_filter(is_literal_integer);

    struct expression
    {
    };

    struct expression_transform
    {
        template<typename T>
        expression operator()(T t) const
        {
            return {};
        }
    };

    // Parser forward declarations

    template<typename T, typename R>
    constexpr halberd::parser::parse_result<expression> parse_expression(halberd::parser::source<T, R>& source);

    template<typename T, typename R>
    constexpr auto parser_expression_v = halberd::parser::combinator_function_v<decltype(parse_expression<T, R>), &parse_expression<T, R>>;

    // Parser rules

    /*
        <expression_terminal> ::= IDENTIFIER | LITERAL_FRACTIONAL | LITERAL_INTEGER
     */

    constexpr auto parser_expression_terminal_v = (
        match_identifier_v |
        match_literal_fractional_v |
        match_literal_integer_v) >> expression_transform();

    /*
        <expression_parentheses> ::= SYMBOL('(') <expression> SYMBOL(')')
     */

    template<typename T, typename R>
    constexpr auto parser_expression_parentheses_v =
        match_symbol_v<halberd::lexer::symbol::bracket_round_open> +
        parser_expression_v<T, R> +
        match_symbol_v<halberd::lexer::symbol::bracket_round_close>;

    // Parser function definitions

    /*
        <expression> ::= <expression_terminal> | <expression_parentheses>
     */

    template<typename T, typename R>
    constexpr halberd::parser::parse_result<expression> parse_expression(halberd::parser::source<T, R>& source)
    {
        constexpr auto parser_expression_primary = (
            parser_expression_terminal_v |
            parser_expression_parentheses_v<T, R>[halberd::parser::index_tag_v<1U>]) >> expression_transform();

        return parser_expression_primary.apply(source);
    }

    // Parser factories

    constexpr auto make_parser_expression()
    {
        using token_t = std::unique_ptr<halberd::lexer::token>;
        using token_ptr_t = halberd::lexer::token*;

        return halberd::parser::combinator_function_v<decltype(parse_expression<token_t, token_ptr_t>), &parse_expression<token_t, token_ptr_t>>;
    }

    constexpr auto make_parser()
    {
        using namespace halberd;

        /*
            Possible tokens are denoted by all caps and include:

            KEYWORD            - lexer::token_identifier_reserved
            IDENTIFIER         - lexer::token_identifier
            LITERAL_FRACTIONAL - lexer::token_literal_fractional
            LITERAL_INTEGER    - lexer::token_literal_integer
            SYMBOL             - lexer::token_symbol
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
            <variable_declaration> ::= KEYWORD('var') <type_integer> IDENTIFIER
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

bool ns::compile_expression(const char* src)
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

    return make_parser_expression().apply(token_source).operator bool();
}
