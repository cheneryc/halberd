#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"


namespace halberd
{
namespace parser
{
    struct combinator_one : combinator
    {
        template<typename T, typename R>
        parse_result<R> apply(source<T, R>& source) const
        {
            auto result = source.next();

            if (!result.second)
            {
                return {};
            }
            else
            {
                try_advance(source);
            }

            return { result.first };
        }
    };

    constexpr combinator_one combinator_one_v;
}
}
