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
        template<typename T, typename TRef>
        parse_result<TRef> apply(source<T, TRef>& source) const
        {
            if (TRef token = source.next())
            {
                //TODO: can't just flush like this unless the source is passed by value (i.e. not shared amongst combinators)
                //source.flush();

                return {
                    std::move(token)
                };
            }

            return {};
        }
    };

    constexpr combinator_one combinator_one_v;
}
}
