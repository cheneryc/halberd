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
        parse_result<> apply(source<T, TRef>& source) const
        {
            bool is_success = true;

            if (TRef token = source.next())
            {
                is_success = false;
            }

            //TODO: can't just flush like this unless the source is passed by value (i.e. not shared amongst combinators)
            //source.flush();

            return { is_success };
        }
    };

    constexpr combinator_end combinator_end_v;
}
}
