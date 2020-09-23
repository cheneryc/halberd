#pragma once

#include "util/optional.h"


namespace halberd
{
namespace parser
{
    //TODO: what about a parse_result for combinator_sequence? Should it have multiple values of various types (std::tuple<util::optional<T1>, util::optional<T2>, ...>)?
    template<typename T>
    class parse_result : public util::optional<T> //TODO: use composition instead of inheritance, private inheritance?
    {
    public:
        using type = T;

        constexpr parse_result(bool is_success = false) noexcept : util::optional<T>(), is_success(is_success)
        {
        }

        parse_result(T&& value) : util::optional<T>(std::move(value)), is_success(true)
        {
        }

        parse_result(const T& value) : util::optional<T>(value), is_success(true)
        {
        }

        //TODO: should operator bool behave differently to that of util::optional? i.e. return is_success rather than whether there is a value or not?
        //TODO: instead of having a separate 'success' field and distinguishing between successful parsing and returning a value why not just have combinator_end return a sentinel type/value?
        //TODO: OR, just have a specialization of parse_result with no arguments e.g. parse_result<>
        const bool is_success;
    };
}
}
