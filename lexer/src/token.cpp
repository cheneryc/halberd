#include <halberd/token.h>

#include <exception> // std::exception

#include <cstddef> // std::size_t


namespace
{
    namespace ns = halberd::lexer;

    float to_float(const std::basic_string<char>& str)
    {
        std::size_t str_pos = 0U;
        const float value = std::stof(str, &str_pos);

        if (str.length() != str_pos)
        {
            throw std::exception();
        }

        return value;
    }
}

ns::token_identifier::token_identifier(std::basic_string<char>&& identifier) : _identifier(std::move(identifier))
{
}

ns::token_identifier_reserved::token_identifier_reserved(keyword kw) : _keyword(kw)
{
}

ns::token_literal_fractional::token_literal_fractional(const std::basic_string<char>& str) : _value(::to_float(str))
{
}

ns::token_symbol::token_symbol(symbol sym) : _symbol(sym)
{
}
