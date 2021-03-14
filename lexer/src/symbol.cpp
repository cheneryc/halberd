#include <halberd/symbol.h>


namespace
{
    namespace ns = halberd::lexer;
}

std::pair<ns::symbol, bool> ns::to_symbol(char ch)
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
            sym = symbol::op_add;
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