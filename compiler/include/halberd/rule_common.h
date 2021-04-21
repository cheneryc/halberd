#pragma once

// halberd::lexer
#include <halberd/keyword.h>
#include <halberd/symbol.h>
#include <halberd/token.h>

// halberd::parser
#include <halberd/combinator_end.h>
#include <halberd/combinator_filter.h>

// halberd::util
#include <halberd/util/type.h>


namespace halberd
{
namespace compiler
{
    /*
        Possible tokens are denoted by all caps and include:

        KEYWORD            - lexer::token_identifier_reserved
        IDENTIFIER         - lexer::token_identifier
        LITERAL_FRACTIONAL - lexer::token_literal_fractional
        LITERAL_INTEGER    - lexer::token_literal_integer
        SYMBOL             - lexer::token_symbol
     */

    constexpr auto parser_end_v = parser::combinator_end_v;

    //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
    inline bool is_identifier(const lexer::token* token) noexcept
    {
        return util::is_type_ptr<lexer::token_identifier>(token);
    }

    constexpr auto match_identifier_v = parser::make_filter(is_identifier);

    //TODO: make constexpr - dynamic_cast is not allowed in constexpr, so need a different approach here
    template<lexer::keyword K>
    bool is_keyword(const lexer::token* token) noexcept
    {
        auto token_identifier_reserved = dynamic_cast<const lexer::token_identifier_reserved*>(token);

        return token_identifier_reserved &&
               token_identifier_reserved->_keyword == K;
    }

    template<lexer::keyword K>
    constexpr auto match_keyword_v = parser::make_filter(is_keyword<K>);

    //TODO: make constexpr - dynamic_cast is not allowed in constexpr, so need a different approach here
    template<lexer::symbol S>
    inline bool is_symbol(const lexer::token* token) noexcept
    {
        auto token_symbol = dynamic_cast<const lexer::token_symbol*>(token);

        return token_symbol &&
               token_symbol->_symbol == S;
    }

    template<lexer::symbol S>
    constexpr auto match_symbol_v = parser::make_filter(is_symbol<S>);

    //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
    inline bool is_literal_fractional(const lexer::token* token) noexcept
    {
        return util::is_type_ptr<lexer::token_literal_fractional>(token);
    }

    constexpr auto match_literal_fractional_v = parser::make_filter(is_literal_fractional);

    //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
    inline bool is_literal_integer(const lexer::token* token) noexcept
    {
        return util::is_type_ptr<lexer::token_literal_integer>(token);
    }

    constexpr auto match_literal_integer_v = parser::make_filter(is_literal_integer);
}
}
