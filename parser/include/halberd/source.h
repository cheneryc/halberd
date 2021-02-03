#pragma once

#include "source_buffer.h"

#include "util/enum_flags.h"

#include <memory> // std::shared_ptr
#include <utility> // std::declval, std::pair
#include <exception> // std::exception
#include <type_traits> // std::decay_t

#include <cstddef> // std::size_t


namespace halberd
{
namespace parser
{
    // Forward declarations

    template<typename T, typename R>
    class source;

    template<typename T, typename R>
    bool try_advance(source<T, R>& source);

    template<typename T, typename R>
    bool try_advance(source<T, R>& source, std::size_t position);

    // Type

    enum class source_flags
    {
        none        = 0x0,
        advanceable = 0x1
    };

    template<typename T, typename R>
    class source
    {
    public:
        source(std::shared_ptr<source_buffer<T, R>> buffer) :
            _buffer(std::move(buffer)),
            _buffer_position(),
            _flags(source_flags::advanceable)
        {
        }

        source(const source& src, source_flags flags) :
            _buffer(src._buffer),
            _buffer_position(src._buffer_position),
            _flags(flags)
        {
        }

        std::pair<R, bool> next()
        {
            auto result = _buffer->at(_buffer_position);

            if (!result.second)
            {
                return { R(), false };
            }
            else
            {
                ++_buffer_position;
            }

            return result;
        }

        bool is_advanceable() const noexcept
        {
            return _flags.has_flag<source_flags::advanceable>();
        }

        source_buffer<T, R>& get_buffer() noexcept
        {
            return *_buffer;
        }

        const source_buffer<T, R>& get_buffer() const noexcept
        {
            return *_buffer;
        }

        std::size_t get_position() const noexcept
        {
            return _buffer_position;
        }

    private:
        friend bool try_advance<T, R>(source& source);
        friend bool try_advance<T, R>(source& source, std::size_t position);

        std::shared_ptr<source_buffer<T, R>> _buffer;
        std::size_t _buffer_position;

        util::enum_flags<source_flags> _flags;
    };

    template<typename T, typename R>
    bool try_advance(source<T, R>& source)
    {
        return try_advance(source, source.get_position());
    }

    template<typename T, typename R>
    bool try_advance(source<T, R>& source, std::size_t position)
    {
        if (position < source._buffer_position)
        {
            throw std::exception();
        }

        const bool is_advanceable = source.is_advanceable();

        if (is_advanceable)
        {
            source.get_buffer().advance_to(position);
        }

        source._buffer_position = position;

        return is_advanceable;
    }

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
