#pragma once

#include "type_list.h"
#include "type_wrapper.h"
#include "value_list.h"


namespace halberd
{
    namespace meta
    {
        template<typename... Ts, typename Fn>
        constexpr auto transform(type_list<Ts...>, Fn fn)
        {
            return concat(fn(type_wrapper_v<Ts>)...);
        }
    }
}
