#pragma once

#include <utility> // std::pair


namespace halberd
{
namespace lexer
{
    enum class symbol
    {
        op_add,
        op_assign
    };

    std::pair<symbol, bool> to_symbol(char ch);
}
}
