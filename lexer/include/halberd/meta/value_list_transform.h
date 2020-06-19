#pragma once

#include "type_list.h"
#include "value_list.h"
#include "value_wrapper.h"


namespace halberd
{
namespace meta
{
    template<typename T, T V, T... Vs, typename Fn>
    constexpr auto transform_values(value_list<T, V, Vs...>, Fn fn) noexcept
    {
        constexpr auto type_wrap = fn(value_wrapper_v<T, V>);
        constexpr auto types_rec = transform_values(value_list_v<T, Vs...>, fn);

        return prepend(type_wrap, types_rec);
    }

    template<typename T, typename Fn>
    constexpr auto transform_values(value_list<T>, Fn fn) noexcept
    {
        return type_list_v<>;
    }
}
}
