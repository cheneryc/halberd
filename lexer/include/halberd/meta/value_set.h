#pragma once

#include "dependent.h"
#include "value_list.h"

#include <type_traits> // std::conditional_t


namespace halberd
{
namespace meta
{
    template<typename T, T... Vs>
    struct value_set
    {
        static_assert(value_list_is_ascending<value_list<T, Vs...>>::value, "value_set: variadic template parameter Vs must contain unique elements in ascending order");
    };

    // Converters

    template<typename T>
    struct value_set_to_list
    {
        static_assert(meta::dependent_false<T>, "value_set_to_list: template parameter T must be value_set");
    };

    template<typename T, T... Vs>
    struct value_set_to_list<value_set<T, Vs...>>
    {
        using type = value_list<T, Vs...>;
    };

    template<typename T>
    using value_set_to_list_t = typename value_set_to_list<T>::type;

    template<typename T>
    struct value_list_to_set
    {
        static_assert(meta::dependent_false<T>, "value_list_to_set: template parameter T must be value_list");
    };

    template<typename T, T... Vs>
    struct value_list_to_set<value_list<T, Vs...>>
    {
        using type = value_set<T, Vs...>;
    };

    template<typename T>
    using value_list_to_set_t = typename value_list_to_set<T>::type;

    // Unary operations

    template<typename T, typename TSet, template<T...> class Fn>
    struct value_set_convert
    {
        static_assert(meta::dependent_false<TSet>, "value_set_convert: template parameter TSet must be value_set");
    };

    template<typename T, T... Vs, template<T...> class Fn>
    struct value_set_convert<T, value_set<T, Vs...>, Fn>
    {
        using type = typename Fn<Vs...>::type;
    };

    // Binary operations

    template<typename T1, typename T2>
    struct value_set_union
    {
        static_assert(meta::dependent_false<T1, T2>, "value_set_union: template parameters T1 and T2 must be value_set");
    };

    template<typename T, T V1, T... V1s, T V2, T... V2s>
    struct value_set_union<value_set<T, V1, V1s...>, value_set<T, V2, V2s...>>
    {
        using type_rem1 = std::conditional_t<V1 <= V2, value_set<T, V1s...>, value_set<T, V1, V1s...>>; // Only retain V1 if V1 is greater than V2
        using type_rem2 = std::conditional_t<V2 <= V1, value_set<T, V2s...>, value_set<T, V2, V2s...>>; // Only retain V2 if V2 is greater than V1

        using type_list_rec = value_set_to_list_t<typename value_set_union<type_rem1, type_rem2>::type>;
        using type_list = typename value_list_concat<value_list<T, ((V1 < V2) ? V1 : V2)>, type_list_rec>::type;
        using type = value_list_to_set_t<type_list>;
    };

    template<typename T, T V, T... Vs>
    struct value_set_union<value_set<T, V, Vs...>, value_set<T>>
    {
        using type = value_set<T, V, Vs...>;
    };

    template<typename T, T V, T... Vs>
    struct value_set_union<value_set<T>, value_set<T, V, Vs...>>
    {
        using type = value_set<T, V, Vs...>;
    };

    template<typename T>
    struct value_set_union<value_set<T>, value_set<T>>
    {
        using type = value_set<T>;
    };
}
}
