#pragma once

#include "parse_result.h"

#include <utility> // std::declval


namespace halberd
{
namespace parser
{
    struct combinator
    {
        template<typename P, typename TSrc>
        using apply_result_t = decltype(std::declval<P>().apply(std::declval<TSrc>()));

        template<std::size_t Idx, typename P, typename TSrc>
        struct apply_result_element
        {
            using type = parse_result_element_t<Idx, apply_result_t<P, TSrc>>;
        };

        template<std::size_t Idx, typename P, typename TSrc>
        using apply_result_element_t = typename apply_result_element<Idx, P, TSrc>::type;
    };
}
}
