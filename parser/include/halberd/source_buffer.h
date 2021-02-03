#pragma once

#include <deque> // std::deque
#include <memory> // std::shared_ptr, std::make_shared
#include <utility> // std::declval, std::pair, std::forward
#include <exception> // std::exception
#include <functional> // std::function
#include <type_traits> // std::decay_t

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename T, typename R>
    class source_buffer
    {
    public:
        template<typename Fn, typename Conv>
        source_buffer(Fn fn, Conv conv) :
            _fn(fn),
            _fn_conv(conv),
            _tokens(),
            _tokens_position()
        {
        }

        std::pair<R, bool> at(std::size_t position)
        {
            if (position < _tokens_position)
            {
                throw std::exception();
            }

            const std::size_t offset = position - _tokens_position;

            while (_tokens.size() <= offset)
            {
                if (auto token = _fn())
                {
                    _tokens.push_back(std::move(token));
                }
                else
                {
                    return { R(), false };
                }
            }

            return { _fn_conv(_tokens[offset]), true };
        }

        void advance(std::size_t distance)
        {
            if (distance > _tokens.size())
            {
                throw std::exception();
            }

            _tokens_position += distance;

            while (distance > 0U)
            {
                --distance; _tokens.pop_front();
            }
        }

        void advance_to(std::size_t position)
        {
            if (position < _tokens_position)
            {
                throw std::exception();
            }

            advance(position - _tokens_position);
        }

        std::size_t get_size() const noexcept
        {
            return _tokens.size();
        }

    private:
        std::function<T()>   _fn;
        std::function<R(T&)> _fn_conv;

        std::deque<T> _tokens;
        std::size_t   _tokens_position;
    };

    namespace detail
    {
        struct pass_through
        {
            template<typename T>
            auto operator()(T&& t) -> decltype(std::forward<T>(t))
            {
                return std::forward<T>(t);
            }
        };
    }

    template<typename Fn,
             typename Conv = detail::pass_through,
             typename T = std::decay_t<decltype(std::declval<Fn>()())>,
             typename R = std::decay_t<decltype(std::declval<Conv>()(std::declval<T&>()))>>
    std::shared_ptr<source_buffer<T, R>> make_source_buffer(Fn fn, Conv conv = Conv())
    {
        return std::make_shared<source_buffer<T, R>>(fn, conv);
    }
}
}
