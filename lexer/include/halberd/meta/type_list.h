#pragma once

#include "type_wrapper.h"


namespace halberd
{
namespace meta
{
    template<typename... Ts>
    struct type_list
    {
    };

    template<typename... Ts>
    constexpr type_list<Ts...> type_list_v;

    // Variadic operations

    template<typename... Ts>
    constexpr auto concat(type_list<Ts...>) noexcept
    {
        return type_list_v<Ts...>;
    }

    template<typename... T1s, typename... T2s, typename... Ts>
    constexpr auto concat(type_list<T1s...>, type_list<T2s...>, Ts...) noexcept
    {
        return concat(type_list_v<T1s..., T2s...>, Ts()...);
    }

    // Binary operations

    template<typename T, typename... Ts>
    constexpr auto prepend(type_wrapper<T>, type_list<Ts...>) noexcept
    {
        return type_list_v<T, Ts...>;
    }

    template<typename T, typename... Ts>
    constexpr auto append(type_list<Ts...>, type_wrapper<T>) noexcept
    {
        return type_list_v<Ts..., T>;
    }

    namespace detail
    {
        template<typename T, typename Fn>
        constexpr auto cartesian_product_row(type_wrapper<T>, type_list<>, Fn fn) noexcept
        {
            return type_list_v<>;
        }

        template<typename T1, typename T2, typename... Ts, typename Fn>
        constexpr auto cartesian_product_row(type_wrapper<T1>, type_list<T2, Ts...>, Fn fn) noexcept
        {
            constexpr auto type_wrap = fn(type_wrapper_v<T1>, type_wrapper_v<T2>);
            constexpr auto types_rec = cartesian_product_row(type_wrapper_v<T1>, type_list_v<Ts...>, fn);

            return prepend(type_wrap, types_rec);
        }
    }

    template<typename... Ts, typename Fn>
    constexpr auto cartesian_product(type_list<>, type_list<Ts...>, Fn fn) noexcept
    {
        return type_list_v<>;
    }

    template<typename T, typename... T1s, typename... T2s, typename Fn>
    constexpr auto cartesian_product(type_list<T, T1s...>, type_list<T2s...>, Fn fn) noexcept
    {
        constexpr auto types_row = detail::cartesian_product_row(type_wrapper_v<T>, type_list_v<T2s...>, fn);
        constexpr auto types_rec = cartesian_product(type_list_v<T1s...>, type_list_v<T2s...>, fn);

        return concat(types_row, types_rec);
    }
}
}
