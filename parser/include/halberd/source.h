#pragma once

#include "source_buffer.h"

#include <memory> // std::shared_ptr
#include <utility> // std::declval, std::pair
#include <type_traits> // std::decay_t

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename T, typename R>
    class source
    {
    public:
        source(std::shared_ptr<source_buffer<T, R>> buffer) : _buffer(std::move(buffer)), _buffer_offset()
        {
        }

        std::pair<R, bool> next()
        {
            auto result = _buffer->at(_buffer_offset);

            if (!result.second)
            {
                return { R(), false };
            }
            else
            {
                ++_buffer_offset;
            }

            return result;
        }

        void flush()
        {
            _buffer->advance(_buffer_offset); //TODO: set _buffer_offset to zero and/or an invalid state?
        }

        std::shared_ptr<source_buffer<T, R>> get_buffer() noexcept
        {
            return _buffer;
        }

        std::size_t get_buffer_offset() const noexcept
        {
            return _buffer_offset;
        }

    private:
        std::shared_ptr<source_buffer<T, R>> _buffer;
        std::size_t _buffer_offset;
    };

    template<typename Fn,
             typename Conv = detail::pass_through,
             typename T = std::decay_t<decltype(std::declval<Fn>()())>,
             typename R = std::decay_t<decltype(std::declval<Conv>()(std::declval<T&>()))>>
    source<T, R> make_source(Fn fn, Conv conv = Conv())
    {
        return { make_source_buffer(fn, conv) };
    }
}
}
