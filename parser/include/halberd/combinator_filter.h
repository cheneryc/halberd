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

        template<typename T, typename R>
        parse_result<R> apply(source<T, R>& source) const
        {
            halberd::parser::source<T, R> source_cpy(source, source_flags::none); // Create a copy of the source that cannot advance its buffer

            if (auto result = combinator_one_v.apply(source_cpy))
            {
                // If the predicate returns a true value then attempt
                // to advance the given source to the copy's position
                if (_pred(result.get()))
                {
                    try_advance(source, source_cpy.get_position());
                }
                else
                {
                    return {};
                }

                return result;
            }

            return {};
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
