#pragma once

#include "combinator.h"
#include "combinator_filter.h"
#include "parse_result.h"
#include "source.h"


namespace halberd
{
namespace parser
{
    template<typename Pred, typename Fn>
    class combinator_transform : public combinator
    {
    public:
        constexpr combinator_transform(Pred pred, Fn fn) noexcept : _filter(std::move(pred)), _fn(std::move(fn))
        {
        }

        //TODO: need to determine the parse_result's value type from Fn
        template<typename T, typename TRef>
        parse_result<TRef> apply(source<T, TRef>& source) const
        {
            if (auto result = _filter.apply(source))
            {
                return { _fn(result.get()) };
            }

            return {};
        }

    private:
        combinator_filter<Pred> _filter;
        Fn _fn;
    };

    template<typename Pred, typename Fn>
    constexpr combinator_transform<Pred, Fn> make_transform(Pred pred, Fn fn) noexcept
    {
        return {
            std::move(pred),
            std::move(fn)
        };
    }
}
}
