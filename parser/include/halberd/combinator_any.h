#pragma once

#include "combinator.h"
#include "parse_result.h"

#include <memory> // std::unique_ptr, std::make_unique


namespace halberd
{
namespace parser
{
    template<typename S, typename R>
    class combinator_any : public combinator
    {
    public:
        template<typename P>
        combinator_any(P parser) : _wrapper(std::make_unique<wrapper_impl<P>>(std::move(parser)))
        {
        }

        parse_result<R> apply(S& source)
        {
            return _wrapper->apply(source);
        }

    private:
        struct wrapper
        {
            virtual ~wrapper() = default;

            virtual parse_result<R> apply(S& source) = 0;
        };

        template<typename P>
        struct wrapper_impl : wrapper
        {
            wrapper_impl(P parser) : _parser(std::move(parser))
            {
            }

            parse_result<R> apply(S& source) override
            {
                return _parser.apply(source);
            }

            P _parser;
        };

        std::unique_ptr<wrapper> _wrapper;
    };
}
}
