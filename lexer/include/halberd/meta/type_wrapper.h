#pragma once


namespace halberd
{
namespace meta
{
    template<typename T>
    struct type_wrapper
    {
    };

    template<typename T>
    constexpr type_wrapper<T> type_wrapper_v;

    // Unary operations

    template<typename T>
    constexpr auto wrap(T&&) noexcept
    {
        return type_wrapper_v<T>;
    }

    template<typename T>
    constexpr T unwrap(type_wrapper<T>) noexcept
    {
        return {};
    }
}
}
