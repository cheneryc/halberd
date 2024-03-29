#pragma once

#include "keyword.h"
#include "symbol.h"

#include <string> // std::basic_string


namespace halberd
{
namespace lexer
{
    struct token
    {
        virtual ~token() = 0;
    };

    struct token_identifier : public token
    {
        token_identifier(std::basic_string<char>&& identifier);

        const std::basic_string<char> _identifier;
    };

    struct token_identifier_reserved : public token
    {
        token_identifier_reserved(keyword kw);

        const keyword _keyword;
    };

    struct token_literal_fractional : public token
    {
        token_literal_fractional(float f) noexcept;
        token_literal_fractional(const std::basic_string<char>& str);

        const float _value;
    };

    struct token_literal_integer : public token
    {
        token_literal_integer(int i) noexcept;
        token_literal_integer(const std::basic_string<char>& str);

        const int _value;
    };

    struct token_symbol : public token
    {
        token_symbol(symbol sym);

        const symbol _symbol;
    };
}
}
