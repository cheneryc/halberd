#pragma once

#include <deque> // std::deque


namespace test
{
    struct scanner
    {
        using token_type = char;

        token_type scan()
        {
            token_type token = {};
            
            if (tokens.size() > 0U)
            {
                token = std::move(tokens.front());
                tokens.pop_front();
            }

            return token;
        }

        std::deque<token_type> tokens;
    };

    template<char Ch>
    constexpr bool is_token(char ch) noexcept
    {
        return Ch == ch;
    }
}
