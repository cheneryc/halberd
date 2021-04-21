#pragma once

#include "combinator.h"
#include "combinator_filter.h"
#include "parse_result.h"
#include "source.h"

#include <utility> // std::declval
#include <type_traits> // std::decay_t


namespace halberd
{
namespace parser
{
    template<typename P, typename Fn>
    class combinator_transform : public combinator
    {
        template<typename TSrc>
        using transform_fn_result_t = decltype(std::declval<Fn>()(get_as_tuple(std::declval<apply_result_t<P, TSrc>>())));

        template<typename TSrc>
        using transform_result_t = parse_result<std::decay_t<transform_fn_result_t<TSrc>>>;

    public:
        constexpr combinator_transform(P parser, Fn fn) noexcept : _parser(std::move(parser)), _fn(std::move(fn))
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const -> transform_result_t<decltype(source)>
        {
            if (auto result = _parser.apply(source))
            {
                return { _fn(get_as_tuple(std::move(result))) };
            }

            return {};
        }

    private:
        P _parser;
        Fn _fn;
    };

    template<typename P, typename Fn>
    constexpr combinator_transform<P, Fn> make_transform(P parser, Fn fn) noexcept
    {
        return {
            std::move(parser),
            std::move(fn)
        };
    }
}
}
