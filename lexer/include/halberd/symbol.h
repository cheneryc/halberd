#pragma once

#include <sstream> // std::basic_istringstream
#include <utility> // std::pair


namespace halberd
{
namespace lexer
{
    enum class symbol
    {
        asterisk, // '*'

        // Signs
        sign_plus,    // '+'
        sign_minus,   // '-'
        sign_percent, // '%'
        sign_equals,  // '='

        // Separators
        semicolon, // ';'

        // Slashes
        slash,      // '/'
        slash_back, // '\'

        // Operators
        op_increment, // '++'
        op_decrement, // '--'

        // Assignment
        assign_plus,     // '+='
        assign_minus,    // '-='
        assign_asterisk, // '*='
        assign_slash,    // '/='
        assign_percent,  // '%='

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
