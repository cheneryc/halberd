#pragma once

#include "symbol_set.h"

#include "meta/value_set.h"


namespace halberd
{
namespace lexer
{
    // Operators

    template<typename TSym, TSym... SymSet1, TSym... SymSet2>
    constexpr auto operator|(const basic_symbol_set<TSym, SymSet1...>&, const basic_symbol_set<TSym, SymSet2...>&)
    {
        using value_set_union_t = typename meta::value_set_union<
            meta::value_set<TSym, SymSet1...>,
            meta::value_set<TSym, SymSet2...>>::type;

        return typename detail::value_set_converter<TSym, value_set_union_t>::type();
    }

    template<typename T1, typename T2>
    constexpr auto operator|(const T1&, const T2&)
    {
        using type_lhs = typename make_symbol_set<T1>::type;
        using type_rhs = typename make_symbol_set<T2>::type;

        return type_lhs() | type_rhs();
    }
}
}
