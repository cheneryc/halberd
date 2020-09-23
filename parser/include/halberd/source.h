#pragma once

#include "source_buffer.h"

#include <memory> // std::shared_ptr
#include <utility> // std::declval

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    template<typename T, typename TRef = T>
    class source
    {
    public:
        source(std::shared_ptr<source_buffer<T, TRef>> buffer) : _buffer(std::move(buffer)), _buffer_offset()
        {
        }

        TRef next()
        {
            if (auto token = _buffer->at(_buffer_offset))
            {
                ++_buffer_offset; return token;
            }

            return {};
        }

        std::shared_ptr<source_buffer<T, TRef>> get_buffer() noexcept
        {
            return _buffer;
        }

        std::size_t get_buffer_offset() const noexcept
        {
            return _buffer_offset;
        }

        void flush()
        {
            _buffer->advance(_buffer_offset); //TODO: set _buffer_offset to zero and/or an invalid state?
        }

    private:
        std::shared_ptr<source_buffer<T, TRef>> _buffer;
        std::size_t _buffer_offset;
    };

    template<typename Fn, typename R = decltype(std::declval<Fn>()())>
    source<R> make_source(Fn fn)
    {
        return { make_source_buffer(fn) };
    }
}
}
