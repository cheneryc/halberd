#pragma once

#include "keyword.h"
#include "symbol.h"

#include <string> // std::string


namespace halberd
{
namespace lexer
{
    struct token
    {
        virtual ~token() = default;
    };

    struct token_identifier : public token
    {
        token_identifier(std::string&& identifier);

        const std::string _identifier;
    };

    struct token_identifier_reserved : public token
    {
        token_identifier_reserved(keyword kw);

        const keyword _keyword;
    };

    struct token_literal_fractional : public token
    {
        token_literal_fractional(const std::string& str);

        const float _value;
    };

    struct token_symbol : public token
    {
        token_symbol(symbol sym);

        const symbol _symbol;
    };
}
}
