#pragma once

#include <type_traits>


namespace halberd
{
namespace util
{
    template<typename E>
    class enum_flags
    {
        using underlying_t = std::underlying_type_t<E>;

    public:
        constexpr enum_flags(E e) noexcept : _u(static_cast<underlying_t>(e))
        {
        }

        constexpr operator E() const
        {
            return static_cast<E>(_u); // Convert the underlying value back to the enumeration type
        }

        bool operator==(const enum_flags<E>& rhs) const
        {
            return _u == rhs._u;
        }

        friend bool operator==(const enum_flags<E> &lhs, const enum_flags<E> &rhs)
        {
            return lhs.operator==(rhs);
        }

        bool operator!=(const enum_flags<E>& rhs) const
        {
            return _u != rhs._u;
        }

        friend bool operator!=(const enum_flags<E> &lhs, const enum_flags<E> &rhs)
        {
            return lhs.operator!=(rhs);
        }

        constexpr enum_flags<E> &operator|=(const enum_flags<E> &rhs)
        {
            _u |= rhs._u;
            return *this;
        }

        constexpr friend enum_flags<E> operator|(const enum_flags<E> &lhs, const enum_flags<E> &rhs)
        {
            enum_flags flags(lhs);

            flags |= rhs;
            return flags;
        }

        constexpr enum_flags<E> &operator&=(const enum_flags<E> &rhs)
        {
            _u &= rhs._u;
            return *this;
        }

        constexpr friend enum_flags<E> operator&(const enum_flags<E> &lhs, const enum_flags<E> &rhs)
        {
            enum_flags flags(lhs);

            flags &= rhs;
            return flags;
        }

        template <E e>
        bool has_flag() const
        {
            return (_u & static_cast<underlying_t>(e)) == static_cast<underlying_t>(e);
        }

        static_assert(std::is_enum<E>::value, "enum_flags: template parameter E must be an enumeration type");

    private:
        underlying_t _u;
    };

    template<typename E, E e>
    constexpr enum_flags<E> enum_flag_v = { e };
}
}
