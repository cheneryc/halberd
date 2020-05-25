#pragma once

#include "symbol_set.h"

#include "meta/dependent.h"

#include <iterator> // std::begin, std::end


namespace halberd
{
namespace lexer
{
    // Array types

    template<typename T>
    struct basic_symbol_set_array
    {
        static_assert(meta::dependent_false<T>, "basic_symbol_set_array: template parameter T must be basic_symbol_set");
    };

    template<typename TSym, TSym... Symbols>
    struct basic_symbol_set_array<basic_symbol_set<TSym, Symbols...>>
    {
        using value_type = TSym;
        using const_iterator = const value_type*;

        constexpr const_iterator begin() const noexcept
        {
            return std::begin(values);
        }

        constexpr const_iterator end() const noexcept
        {
            return std::end(values);
        }

        static constexpr value_type values[] = { Symbols... };
    };
    
    template<typename TSym, TSym... Symbols>
    constexpr TSym basic_symbol_set_array<basic_symbol_set<TSym, Symbols...>>::values[]; // Out-of-line static member definition

    // Converters

    template<typename TSym, TSym... Symbols>
    constexpr auto to_symbol_set_array(basic_symbol_set<TSym, Symbols...>) noexcept
    {
        return basic_symbol_set_array<basic_symbol_set<TSym, Symbols...>>();
    }
}
}
