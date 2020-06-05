#pragma once

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

    template<typename TSym, TSym... Symbols>
    struct basic_symbol_set : private meta::value_set<TSym, Symbols...> // Derives from meta::value_set to ensure that Symbols contains ascending values
    {
        static_assert(sizeof...(Symbols) > 0U, "basic_symbol_set: variadic template parameter Symbols must contain at least one value");
    };

    // Variables

    template<typename TSym, TSym Sym>
    constexpr basic_symbol<TSym, Sym> basic_symbol_v;

    template<typename TSym, TSym Sym, std::size_t N>
    constexpr basic_symbol_sequence<TSym, Sym, N> basic_symbol_sequence_v;

    template<typename TSym, TSym SymFrom, TSym SymTo>
    constexpr basic_symbol_range<TSym, SymFrom, SymTo> basic_symbol_range_v;

    template<typename TSym, TSym... Symbols>
    constexpr basic_symbol_set<TSym, Symbols...> basic_symbol_set_v;

    // Converters

    namespace detail
    {
        template<typename TSym, TSym... Symbols>
        constexpr auto to_symbol_set(meta::value_set<TSym, Symbols...>) noexcept
        {
            return basic_symbol_set_v<TSym, Symbols...>;
        }

        template<typename TSym, TSym... Symbols>
        constexpr auto to_symbol_set(meta::value_list<TSym, Symbols...>) noexcept
        {
            return basic_symbol_set_v<TSym, Symbols...>;
        }
    }

    template<typename TSym, TSym Sym>
    constexpr auto make_symbol_set(basic_symbol<TSym, Sym>) noexcept
    {
        return basic_symbol_set_v<TSym, Sym>;
    }

    template<typename TSym, TSym Sym, std::size_t N>
    constexpr auto make_symbol_set(basic_symbol_sequence<TSym, Sym, N>) noexcept
    {
        return detail::to_symbol_set(meta::generate_n<TSym, Sym, N>());
    }

    template<typename TSym, TSym SymFrom, TSym SymTo>
    constexpr auto make_symbol_set(basic_symbol_range<TSym, SymFrom, SymTo>) noexcept
    {
        return make_symbol_set(basic_symbol_sequence_v<TSym, SymFrom, (SymTo - SymFrom) + 1U>); // The range is inclusive - adding one includes SymTo in the generated sequence
    }

    template<typename TSym, TSym... Symbols>
    constexpr auto make_symbol_set(basic_symbol_set<TSym, Symbols...> symbols) noexcept
    {
        return symbols; // Pass through for basic_symbol_set
    }
}
}
