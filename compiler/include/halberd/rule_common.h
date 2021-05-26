#pragma once

// halberd::lexer
#include <halberd/keyword.h>
#include <halberd/symbol.h>
#include <halberd/token.h>

// halberd::parser
#include <halberd/combinator_end.h>
#include <halberd/combinator_filter.h>
#include <halberd/combinator_transform.h>

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

        E represents the empty string
     */

    constexpr auto parser_end_v = parser::combinator_end_v;

    namespace detail
    {
        //TODO: make constexpr - dynamic_cast is not allowed in constexpr, so need a different approach here
        template<lexer::keyword K>
        bool is_keyword(const lexer::token* token) noexcept
        {
            auto token_identifier_reserved = dynamic_cast<const lexer::token_identifier_reserved*>(token);

            return token_identifier_reserved &&
                   token_identifier_reserved->_keyword == K;
        }

        //TODO: make constexpr - dynamic_cast is not allowed in constexpr, so need a different approach here
        template<lexer::symbol S>
        inline bool is_symbol(const lexer::token* token) noexcept
        {
            auto token_symbol = dynamic_cast<const lexer::token_symbol*>(token);

            return token_symbol &&
                   token_symbol->_symbol == S;
        }

        //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
        inline bool is_identifier(const lexer::token* token) noexcept
        {
            return util::is_type_ptr<lexer::token_identifier>(token);
        }

        //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
        inline bool is_literal_fractional(const lexer::token* token) noexcept
        {
            return util::is_type_ptr<lexer::token_literal_fractional>(token);
        }

        //TODO: make constexpr - is_type_ptr uses dynamic_cast, which is not allowed in constexpr, so need a different approach here
        inline bool is_literal_integer(const lexer::token* token) noexcept
        {
            return util::is_type_ptr<lexer::token_literal_integer>(token);
        }

        inline lexer::keyword match_keyword_transform(std::tuple<const lexer::token*> value) noexcept
        {
            auto token = std::get<0U>(value);
            auto token_identifier_reserved = dynamic_cast<const lexer::token_identifier_reserved*>(token);

            return token_identifier_reserved->_keyword;
        }

        inline lexer::symbol match_symbol_transform(std::tuple<const lexer::token*> value) noexcept
        {
            auto token = std::get<0U>(value);
            auto token_symbol = dynamic_cast<const lexer::token_symbol*>(token);

            return token_symbol->_symbol;
        }

        inline std::string match_identifier_transform(std::tuple<const lexer::token*> value) noexcept
        {
            auto token = std::get<0U>(value);
            auto token_identifier = dynamic_cast<const lexer::token_identifier*>(token);

            return token_identifier->_identifier;
        }

        inline float match_literal_fractional_transform(std::tuple<const lexer::token*> value) noexcept
        {
            auto token = std::get<0U>(value);
            auto token_literal_fractional = dynamic_cast<const lexer::token_literal_fractional*>(token);

            return token_literal_fractional->_value;
        }

        inline int match_literal_integer_transform(std::tuple<const lexer::token*> value) noexcept
        {
            auto token = std::get<0U>(value);
            auto token_literal_integer = dynamic_cast<const lexer::token_literal_integer*>(token);

            return token_literal_integer->_value;
        }
    }

    // Filters

    template<lexer::keyword K>
    constexpr auto filter_keyword_v = parser::make_filter(detail::is_keyword<K>);

    template<lexer::symbol S>
    constexpr auto filter_symbol_v = parser::make_filter(detail::is_symbol<S>);

    constexpr auto filter_identifier_v = parser::make_filter(detail::is_identifier);
    constexpr auto filter_literal_fractional_v = parser::make_filter(detail::is_literal_fractional);
    constexpr auto filter_literal_integer_v = parser::make_filter(detail::is_literal_integer);

    // Transforms

    template<lexer::keyword K>
    constexpr auto match_keyword_v = parser::make_transform(filter_keyword_v<K>, detail::match_keyword_transform);

    template<lexer::symbol S>
    constexpr auto match_symbol_v = parser::make_transform(filter_symbol_v<S>, detail::match_symbol_transform);

    constexpr auto match_identifier_v = parser::make_transform(filter_identifier_v, detail::match_identifier_transform);
    constexpr auto match_literal_fractional_v = parser::make_transform(filter_literal_fractional_v, detail::match_literal_fractional_transform);
    constexpr auto match_literal_integer_v = parser::make_transform(filter_literal_integer_v, detail::match_literal_integer_transform);
}
}
