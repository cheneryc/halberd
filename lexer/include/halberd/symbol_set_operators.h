#pragma once

#include "symbol_set.h"

// halberd::meta
#include <halberd/meta/value_set.h>


namespace halberd
{
namespace lexer
{
    // Operators

    template<typename TSym, TSym... Symbols1, TSym... Symbols2>
    constexpr auto operator|(basic_symbol_set<TSym, Symbols1...>, basic_symbol_set<TSym, Symbols2...>)
    {
        return detail::to_symbol_set(
            meta::value_set_v<TSym, Symbols1...> |
            meta::value_set_v<TSym, Symbols2...>);
    }

    template<typename T1, typename T2>
    constexpr auto operator|(T1 t1, T2 t2)
    {
        return make_symbol_set(t1) | make_symbol_set(t2);
    }
}
}
