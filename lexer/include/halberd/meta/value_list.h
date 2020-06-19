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

    template<typename T, T... Vs>
    constexpr value_list<T, Vs...> value_list_v;

    // Traits

    template<typename T>
    struct is_ascending
    {
        static_assert(meta::dependent_false<T>, "is_ascending: template parameter T must be value_list");
    };

    template<typename T>
    struct is_ascending<value_list<T>> : std::true_type
    {
    };

    template<typename T, T V>
    struct is_ascending<value_list<T, V>> : std::true_type
    {
    };

    template<typename T, T V1, T V2, T... Vs>
    struct is_ascending<value_list<T, V1, V2, Vs...>> : std::conditional_t<V1 < V2, is_ascending<value_list<T, V2, Vs...>>, std::false_type>
    {
    };

    // Variadic operations
    
    template<typename T, T... Vs>
    constexpr auto concat(value_list<T, Vs...>) noexcept
    {
        return value_list_v<T, Vs...>;
    }

    template<typename T, T... V1s, T... V2s, typename... Ts>
    constexpr auto concat(value_list<T, V1s...>, value_list<T, V2s...>, Ts...) noexcept
    {
        return concat(value_list_v<T, V1s..., V2s...>, Ts()...);
    }

    // Search operations

    template<typename T, T V, T... Vs>
    constexpr bool contains(value_list<T, V, Vs...>, T value) noexcept
    {
        return (V == value) || contains(value_list_v<T, Vs...>, value);
    }

    template<typename T>
    constexpr bool contains(value_list<T>, T value) noexcept
    {
        return false;
    }

    // Creation

    namespace detail
    {
        template<std::size_t N>
        struct size_tag
        {
        };

        template<std::size_t N>
        constexpr size_tag<N> size_tag_v;

        template<typename T, T V, std::size_t N>
        constexpr auto generate_n(size_tag<N>) noexcept
        {
            static_assert(N > 0U, "generate_n: template parameter N must be greater than zero");

            constexpr auto values_lo = generate_n<T, V>(size_tag_v<N / 2U>);
            constexpr auto values_hi = generate_n<T, V + (N / 2U)>(size_tag_v<N - (N / 2U)>);

            return concat(values_lo, values_hi);
        }

        template<typename T, T V>
        constexpr auto generate_n(size_tag<1U>) noexcept
        {
            return value_list_v<T, V>;
        }
    }

    template<typename T, T V, std::size_t N>
    constexpr auto generate_n() noexcept
    {
        return detail::generate_n<T, V>(detail::size_tag_v<N>);
    }
}
}
