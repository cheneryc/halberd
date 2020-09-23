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
        template<typename T, typename TRef>
        parse_result<TRef> apply(source<T, TRef>& source) const
        {
            if (TRef token = source.next())
            {
                return {};
            }

            //TODO: can't just flush like this unless the source is passed by value (i.e. not shared amongst combinators)
            //source.flush();

            return { true };
        }
    };

    constexpr combinator_end combinator_end_v;
}
}
