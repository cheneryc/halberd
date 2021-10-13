#pragma once

#include "value_list.h"

#include <type_traits> // std::conditional_t


namespace halberd
{
namespace meta
{
    template<typename T, T... Vs>
    struct value_set
    {
        static_assert(is_ascending<value_list<T, Vs...>>::value, "value_set: variadic template parameter Vs must contain unique elements in ascending order");
    };

    template<typename T, T... Vs>
    constexpr value_set<T, Vs...> value_set_v;

    // Conversion

    template<typename T, T... Vs>
    constexpr auto set_to_list(value_set<T, Vs...>) noexcept
    {
        return value_list_v<T, Vs...>;
    }

    template<typename T, T... Vs>
    constexpr auto list_to_set(value_list<T, Vs...>) noexcept
    {
        return value_set_v<T, Vs...>;
    }

    // Binary operators

    template<typename T, T V1, T... V1s, T V2, T... V2s>
    constexpr auto operator|(value_set<T, V1, V1s...>, value_set<T, V2, V2s...>) noexcept
    {
        using type_rem1 = std::conditional_t<V1 <= V2, value_set<T, V1s...>, value_set<T, V1, V1s...>>; // Only retain V1 if V1 is greater than V2
        using type_rem2 = std::conditional_t<V2 <= V1, value_set<T, V2s...>, value_set<T, V2, V2s...>>; // Only retain V2 if V2 is greater than V1

        constexpr auto list_rec = set_to_list(type_rem1() | type_rem2());
        constexpr auto list = concat(value_list_v<T, ((V1 < V2) ? V1 : V2)>, list_rec);

        return list_to_set(list);
    }

    template<typename T, T V, T... Vs>
    constexpr auto operator|(value_set<T, V, Vs...>, value_set<T>) noexcept
    {
        return value_set_v<T, V, Vs...>;
    }

    template<typename T, T V, T... Vs>
    constexpr auto operator|(value_set<T>, value_set<T, V, Vs...>) noexcept
    {
        return value_set_v<T, V, Vs...>;
    }

    template<typename T>
    constexpr auto operator|(value_set<T>, value_set<T>) noexcept
    {
        return value_set_v<T>;
    }
}
}
