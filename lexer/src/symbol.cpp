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
        case '+':
            sym = symbol::op_add;
            break;
        case '=':
            sym = symbol::op_assign;
            break;
        default:
            return { sym, false }; // The returned symbol value is undefined
    }

    return { sym, true };
}