#pragma once

#include <sstream> // std::basic_istringstream
#include <utility> // std::pair


namespace halberd
{
namespace lexer
{
    enum class symbol
    {
        // Signs
        sign_plus,      // '+'
        sign_minus,     // '-'
        sign_equals,    // '='

        // Operators
        op_increment, // '++'
        op_decrement, // '--'

        // Parentheses
        bracket_round_open,  // '('
        bracket_round_close, // ')'

        // Braces
        bracket_curly_open,  // '{'
        bracket_curly_close, // '}'
    };

    std::pair<symbol, bool> to_symbol(char ch, std::istringstream& ss);
}
}
