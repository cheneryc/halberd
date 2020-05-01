#pragma once

#include "dependent.h"

#include <type_traits> // std::conditional_t, std::true_type, std::false_type


namespace halberd
{
namespace meta
{
    template<typename T, T... Vs>
    struct value_list
    {
    };

    // Traits

    template<typename T>
    struct value_list_is_ascending
    {
        static_assert(meta::dependent_false<T>, "value_list_is_ascending: template parameter T must be value_list");
    };

    template<typename T>
    struct value_list_is_ascending<value_list<T>> : std::true_type
    {
    };

    template<typename T, T V>
    struct value_list_is_ascending<value_list<T, V>> : std::true_type
    {
    };

    template<typename T, T V1, T V2, T... Vs>
    struct value_list_is_ascending<value_list<T, V1, V2, Vs...>> : std::conditional_t<V1 < V2, value_list_is_ascending<value_list<T, V2, Vs...>>, std::false_type>
    {
    };

    // Unary operations

    template<typename T, typename TList, template<T...> class Fn>
    struct value_list_convert
    {
        static_assert(meta::dependent_false<TList>, "value_list_convert: template parameter TList must be value_list");
    };

    template<typename T, T... Vs, template<T...> class Fn>
    struct value_list_convert<T, value_list<T, Vs...>, Fn>
    {
        using type = typename Fn<Vs...>::type;
    };

    // Variadic operations

    template<typename... Ts>
    struct value_list_concat
    {
        static_assert(meta::dependent_false<Ts...>, "value_list_concat: template parameter pack Ts must be comprised of value_list types");
    };

    template<typename T, T... Vs>
    struct value_list_concat<value_list<T, Vs...>>
    {
        using type = value_list<T, Vs...>;
    };

    template<typename T, T... V1s, T... V2s, typename... Ts>
    struct value_list_concat<value_list<T, V1s...>, value_list<T, V2s...>, Ts...>
    {
        using type = typename value_list_concat<value_list<T, V1s..., V2s...>, Ts...>::type;
    };

    // Creation

    template<typename T, T V, std::size_t N>
    struct value_list_generate
    {
        using type_lo = typename value_list_generate<T, V, N / 2U>::type;
        using type_hi = typename value_list_generate<T, V + (N / 2U), N - (N / 2U)>::type;

        using type = typename value_list_concat<type_lo, type_hi>::type;
    };

    template<typename T, T V>
    struct value_list_generate<T, V, 1U>
    {
        using type = value_list<T, V>;
    };
}
}
