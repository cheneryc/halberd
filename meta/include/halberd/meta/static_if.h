#pragma once

#include <type_traits> // std::integral_constant, std::true_type, std::false_type


namespace halberd
{
namespace meta
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
}
