#pragma once

#include "combinator.h"
#include "parse_result.h"
#include "source.h"

#include <tuple> // std::tuple
#include <vector> // std::vector


namespace halberd
{
namespace parser
{
    template<typename P>
    class combinator_repeat : combinator
    {
        template<typename>
        struct repeat_container;

        template<typename T>
        struct repeat_container<parse_result<T>>
        {
            using type = std::vector<T>;
        };

        template<typename... Ts>
        struct repeat_container<parse_result<Ts...>>
        {
            using type = std::vector<std::tuple<Ts...>>;
        };

        template<typename TSrc>
        using repeat_result_t = parse_result<typename repeat_container<apply_result_t<P, TSrc>>::type>;

    public:
        constexpr combinator_repeat(P parser) noexcept : _parser(std::move(parser))
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const -> repeat_result_t<decltype(source)>
        {
            repeat_result_t<decltype(source)> results({}); // Construct a 'success' parse_result instance containing an empty vector

            while (auto result = _parser.apply(source))
            {
                results.get().push_back(get_value(std::move(result)));
            }

            return results;
        }

    private:
        P _parser;
    };

    template<typename P>
    constexpr combinator_repeat<P> make_repeat(P parser) noexcept
    {
        return { std::move(parser) };
    }
}
}
