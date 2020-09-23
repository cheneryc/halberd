#pragma once

#include <exception> // std::exception
#include <utility> // std::move


namespace halberd
{
namespace util
{
    template<typename T>
    class optional
    {
    public:
        constexpr optional() noexcept : _value_default(), _has_value(false)
        {
        }

        optional(T&& value) : _value(std::move(value)), _has_value(true)
        {
        }

        optional(const T& value) : _value(value), _has_value(true)
        {
        }

        constexpr bool has_value() const noexcept
        {
            return _has_value;
        }

        constexpr T& get()
        {
            if (!_has_value)
            {
                throw std::exception();
            }

            return _value;
        }

        constexpr const T& get() const
        {
            if (!_has_value)
            {
                throw std::exception();
            }

            return _value;
        }

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

    private:
        // Anonymous union - members are injected into the enclosing scope
        union
        {
            // A 'default' member is required as T may not be default constructible or assignable
            struct
            {
            } _value_default;

            T _value;
        };

        const bool _has_value;
    };
}
}
