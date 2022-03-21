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

        optional(optional&& other) : _value_default(), _has_value(other._has_value)
        {
            if (other._has_value)
            {
                // _value_default is trivially destructible so no need to explicitly call the destructor here
                // _value requires explicit constructor call (via placement new) to activate the member
                new (&_value) T(std::move(other._value)); // A moved-from optional still contains a value, but the value itself is moved-from
            }
        }

        optional(const optional& other) : _value_default(), _has_value(other._has_value)
        {
            if (other._has_value)
            {
                // _value_default is trivially destructible so no need to explicitly call the destructor here
                // _value requires explicit constructor call (via placement new) to activate the member
                new (&_value) T(other._value);
            }
        }

        // Allow class template instantiations with a different template parameter to access
        // this instantiations's private members from within the covariant move constructor
        template<typename U>
        template<typename V>
        friend optional<U>::optional(optional<V>&&);

        template<typename U>
        optional(optional<U>&& other) : _value_default(), _has_value(other._has_value)
        {
            if (other._has_value)
            {
                // _value_default is trivially destructible so no need to explicitly call the destructor here
                // _value requires explicit constructor call (via placement new) to activate the member
                new (&_value) T(std::move(other._value)); // A moved-from optional still contains a value, but the value itself is moved-from
            }
        }

        // Allow class template instantiations with a different template parameter to access
        // this instantiations's private members from within the covariant move constructor
        template<typename U>
        template<typename V>
        friend optional<U>::optional(const optional<V>&);

        template<typename U>
        optional(const optional<U>& other) : _value_default(), _has_value(other._has_value)
        {
            if (other._has_value)
            {
                // _value_default is trivially destructible so no need to explicitly call the destructor here
                // _value requires explicit constructor call (via placement new) to activate the member
                new (&_value) T(other._value);
            }
        }

        ~optional() noexcept
        {
            if (_has_value)
            {
                _value.~T();
            }
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

        optional& operator=(T&& value)
        {
            if (_has_value)
            {
                _value.~T();
            }

            // _value requires explicit constructor call (via placement new) to activate the member
            new (&_value) T(std::move(_value));
            _has_value = true;

            return *this;
        }

        optional& operator=(const T& value)
        {
            if (_has_value)
            {
                _value.~T();
            }

            // _value requires explicit constructor call (via placement new) to activate the member
            new (&_value) T(_value);
            _has_value = true;

            return *this;
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

        bool _has_value;
    };
}
}
