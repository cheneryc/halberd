#include <halberd/symbol.h>


namespace
{
    namespace ns = halberd::lexer;

    bool try_match(std::istringstream& ss, char ch)
    {
        const bool is_match = ss.peek() == ch;

        if (is_match)
        {
            ss.get(ch);
        }

        return is_match;
    }
}

std::pair<ns::symbol, bool> ns::to_symbol(char ch, std::istringstream& ss)
{
    symbol sym;

    switch (ch)
    {
        case '(': // 40
            sym = symbol::bracket_round_open;
            break;
        case ')': // 41
            sym = symbol::bracket_round_close;
            break;
        case '+': // 43
            sym = try_match(ss, '+') ? symbol::op_increment : symbol::op_add;
            break;
        case '-': // 45
            sym = try_match(ss, '-') ? symbol::op_decrement : symbol::op_subtract;
            break;
        case '=': // 61
            sym = symbol::op_assign;
            break;
        case '{': // 123
            sym = symbol::bracket_curly_open;
            break;
        case '}': // 125
            sym = symbol::bracket_curly_close;
            break;
        default:
            return { sym, false }; // The returned symbol value is undefined
    }

    return { sym, true };
}
