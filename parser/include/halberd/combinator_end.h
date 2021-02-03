#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"


namespace halberd
{
namespace parser
{
    // Matches the end of the token sequence
    struct combinator_end : combinator
    {
        template<typename T, typename R>
        parse_result<> apply(source<T, R>& source) const
        {
            auto result = source.next();

            // The parser succeeds if the source has no more tokens
            if (!result.second)
            {
                try_advance(source);
            }

            return { !result.second };
        }
    };

    constexpr combinator_end combinator_end_v;
}
}
