#pragma once

#include <deque> // std::deque
#include <memory> // std::shared_ptr
#include <utility> // std::declval
#include <functional> // std::function

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename T, typename TRef = T>
    class source_buffer
    {
    public:
        template<typename Fn>
        source_buffer(Fn fn) : _fn(fn), _tokens()
        {
        }

        TRef at(std::size_t offset)
        {
            while (_tokens.size() <= offset)
            {
                if (auto token = _fn())
                {
                    _tokens.push_back(std::move(token));
                }
                else
                {
                    return {};
                }
            }

            return _tokens[offset];
        }

        void advance(std::size_t distance)
        {
            while (distance > 0U)
            {
                --distance; _tokens.pop_front();
            }
        }

    private:
        std::function<T()> _fn;
        std::deque<T> _tokens;
    };

    template<typename Fn, typename R = decltype(std::declval<Fn>()())>
    std::shared_ptr<source_buffer<R>> make_source_buffer(Fn fn)
    {
        return std::make_shared<source_buffer<R>>(fn);
    }
}
}
