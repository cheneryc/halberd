#pragma once

#include "type_wrapper.h"

#include <type_traits> // std::integral_constant, std::true_type, std::false_type


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

    // Sequence operations

    namespace detail
    {
        template<typename TT, typename TF>
        constexpr auto static_if(std::true_type, TT t, TF f)
        {
            return t;
        }

        template<typename TT, typename TF>
        constexpr auto static_if(std::false_type, TT t, TF f)
        {
            return f;
        }

        template<bool B, typename TT, typename TF>
        constexpr auto static_if(TT t, TF f)
        {
            return static_if(std::integral_constant<bool, B>(), t, f);
        }
    }

    template<typename T, typename... Ts, typename TEnd, typename Fn>
    constexpr auto find_if(type_list<T, Ts...>, type_wrapper<TEnd>, Fn fn) noexcept
    {
        constexpr auto type_wrap = type_wrapper_v<T>;
        constexpr auto type_rec = find_if(type_list_v<Ts...>, type_wrapper_v<TEnd>, fn); //C++17: unfortunately this calls fn for all types in the list regardless of whether a match has been found, if constexpr would allow short circuiting here

        return detail::static_if<fn(type_wrap)>(type_wrap, type_rec);
    }

    template<typename TEnd, typename Fn>
    constexpr auto find_if(type_list<>, type_wrapper<TEnd>, Fn fn) noexcept
    {
        return type_wrapper_v<TEnd>;
    }

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

    template<typename... Ts>
    constexpr auto concat(type_wrapper<Ts>...) noexcept
    {
        return type_list_v<Ts...>;
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
