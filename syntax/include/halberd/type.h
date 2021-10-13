#pragma once


namespace halberd
{
namespace syntax
{
    enum class type_id
    {
        type_void,

        // boolean
        type_bool,

        // integer
        type_i8,
        type_i16,
        type_i32,
        type_i64,

        // fractional
        type_f32,
        type_f64
    };

    struct type
    {
        const type_id id;
    };

    constexpr bool operator==(const type& lhs, const type& rhs) noexcept
    {
        return lhs.id == rhs.id;
    }

    constexpr bool operator!=(const type& lhs, const type& rhs) noexcept
    {
        return !(lhs == rhs);
    }
}
}
