#pragma once

#include "combinator.h"
#include "source.h"

#include <type_traits> // std::decay_t


namespace halberd
{
namespace parser
{
    template<typename Fn, std::decay_t<Fn> fn>
    class combinator_function : public combinator
    {
    public:
        template<typename T, typename R>
        auto apply(source<T, R>& source) const
        {
            return fn(source);
        }
    };

    template<typename Fn, std::decay_t<Fn> fn>
    class combinator_function<Fn, fn> combinator_function_v;
}
}
