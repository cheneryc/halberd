#pragma once


namespace halberd
{
namespace meta
{
    template<typename T, T V>
    struct value_wrapper
    {
    };

    template<typename T, T V>
    constexpr value_wrapper<T, V> value_wrapper_v;
}
}
