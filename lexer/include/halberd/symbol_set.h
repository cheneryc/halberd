#pragma once

#include "meta/dependent.h"
#include "meta/value_list.h"
#include "meta/value_set.h"


namespace halberd
{
namespace lexer
{
    // Symbol types

    template<typename TSym, TSym Sym>
    struct basic_symbol
    {
    };

    template<typename TSym, TSym Sym, std::size_t N>
    struct basic_symbol_sequence
    {
        static_assert(N > 0U, "basic_symbol_sequence: template parameter N must be greater than zero");
    };

    template<typename TSym, TSym SymFrom, TSym SymTo> // Range is inclusive (SymTo is final symbol in the range)
    struct basic_symbol_range
    {
        static_assert(SymTo >= SymFrom, "basic_symbol_range: template parameter SymTo must be greater than or equal to SymFrom");
    };

    template<typename TSym, TSym... SymSet>
    struct basic_symbol_set : private meta::value_set<TSym, SymSet...> // Derives from meta::value_set to ensure that SymSet contains ascending values
    {
        static_assert(sizeof...(SymSet) > 0U, "basic_symbol_set: variadic template parameter SymSet must contain at least one value");
    };

    // Variables

    template<typename TSym, TSym Sym>
    constexpr basic_symbol<TSym, Sym> basic_symbol_v;

    template<typename TSym, TSym Sym, std::size_t N>
    constexpr basic_symbol_sequence<TSym, Sym, N> basic_symbol_sequence_v;

    template<typename TSym, TSym SymFrom, TSym SymTo>
    constexpr basic_symbol_range<TSym, SymFrom, SymTo> basic_symbol_range_v;

    template<typename TSym, TSym... SymSet>
    constexpr basic_symbol_set<TSym, SymSet...> basic_symbol_set_v;

    // Converters

    namespace detail
    {
        template<typename TSym, typename TSet>
        struct value_set_converter
        {
            template<TSym... SymSet>
            struct fn_convert
            {
                using type = basic_symbol_set<TSym, SymSet...>;
            };

            using type = typename meta::value_set_convert<TSym, TSet, fn_convert>::type;
        };

        template<typename TSym, typename TList>
        struct value_list_converter
        {
            template<TSym... SymSet>
            struct fn_convert
            {
                using type = basic_symbol_set<TSym, SymSet...>;
            };

            using type = typename meta::value_list_convert<TSym, TList, fn_convert>::type;
        };
    }

    template<typename T>
    struct make_symbol_set
    {
        static_assert(meta::dependent_false<T>, "make_symbol_set: template parameter T must be basic_symbol, basic_symbol_set, basic_symbol_range or basic_symbol_sequence");
    };

    template<typename TSym, TSym Sym>
    struct make_symbol_set<basic_symbol<TSym, Sym>>
    {
        using type = basic_symbol_set<TSym, Sym>;
    };

    template<typename TSym, TSym Sym, std::size_t N>
    struct make_symbol_set<basic_symbol_sequence<TSym, Sym, N>>
    {
        using type = typename detail::value_list_converter<TSym, typename meta::value_list_generate<TSym, Sym, N>::type>::type;
    };

    template<typename TSym, TSym SymFrom, TSym SymTo>
    struct make_symbol_set<basic_symbol_range<TSym, SymFrom, SymTo>>
    {
        using type = typename make_symbol_set<basic_symbol_sequence<TSym, SymFrom, (SymTo - SymFrom) + 1U>>::type; // The range is inclusive - adding one includes SymTo in the generated sequence
    };

    template<typename TSym, TSym... SymSet>
    struct make_symbol_set<basic_symbol_set<TSym, SymSet...>>
    {
        using type = basic_symbol_set<TSym, SymSet...>; // Pass through for basic_symbol_set
    };
}
}
