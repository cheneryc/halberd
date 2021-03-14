#pragma once

#include "combinator.h"
#include "source.h"


namespace halberd
{
namespace parser
{
    template<typename Fn>
    class combinator_function : public combinator
    {
    public:
        constexpr combinator_function(Fn fn) noexcept : _fn(fn)
        {
        }

        template<typename T, typename R>
        auto apply(source<T, R>& source) const
        {
            return _fn(source);
        }

    private:
        Fn _fn;
    };

    template<typename S, typename R>
    constexpr auto make_function(R (*fn)(S&)) -> combinator_function<R (*)(S&)>
    {
        return { fn };
    }
}
}
