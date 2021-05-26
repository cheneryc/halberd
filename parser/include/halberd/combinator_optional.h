#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include "util/optional.h"

#include <tuple> // std::tuple


namespace halberd
{
namespace parser
{
    template<typename P>
    class combinator_optional : combinator
    {
        template<typename>
        struct optional_value;

        template<typename T>
        struct optional_value<parse_result<T>>
        {
            using type = util::optional<T>;
        };

        template<typename... Ts>
        struct optional_value<parse_result<Ts...>>
        {
            using type = util::optional<std::tuple<Ts...>>;
        };

        template<typename TSrc>
        using optional_result_t = parse_result<typename optional_value<apply_result_t<P, TSrc>>::type>;

    public:
        constexpr combinator_optional(P parser) noexcept : _parser(std::move(parser))
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const -> optional_result_t<decltype(source)>
        {
            if (auto result = _parser.apply(source))
            {
                return { { get_as_tuple(std::move(result)) } };
            }

            return { {} }; // Construct a 'success' parse_result containing an empty util::optional
        }

    private:
        P _parser;
    };

    template<typename P>
    constexpr combinator_optional<P> make_optional(P parser) noexcept
    {
        return { std::move(parser) };
    }
}
}
