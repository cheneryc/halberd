#pragma once

#include "combinator.h"
#include "combinator_one.h"
#include "parse_result.h"
#include "source.h"


namespace halberd
{
namespace parser
{
    template<typename Pred>
    class combinator_filter : public combinator
    {
    public:
        constexpr combinator_filter(Pred pred) noexcept : _pred(std::move(pred))
        {
        }

        template<typename T, typename TRef>
        parse_result<TRef> apply(source<T, TRef>& source) const
        {
            auto result = combinator_one_v.apply(source);

            if (!result.has_value() || _pred(result.get()))
            {
                return result;
            }

            return { false };
        }

    private:
        Pred _pred;
    };

    template<typename Pred>
    constexpr combinator_filter<Pred> make_filter(Pred pred) noexcept
    {
        return { std::move(pred) };
    }
}
}
