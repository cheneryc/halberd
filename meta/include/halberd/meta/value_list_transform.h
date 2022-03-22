#pragma once

#include "value_list.h"
#include "value_wrapper.h"
#include "type_list.h"


namespace halberd
{
namespace meta
{
    template<typename T, T... Vs, typename Fn>
    constexpr auto transform(value_list<T, Vs...>, Fn fn)
    {
        return concat(fn(value_wrapper_v<T, Vs>)...);
    }
}
}
