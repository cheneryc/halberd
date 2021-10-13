#pragma once


namespace halberd
{
namespace meta
{
    template<typename T, T t, typename... TDeps>
    constexpr T dependent_value = t;

    template<typename... TDeps>
    constexpr bool dependent_true = dependent_value<bool, true, TDeps...>;

    template<typename... TDeps>
    constexpr bool dependent_false = dependent_value<bool, false, TDeps...>;
}
}
