#pragma once


namespace halberd
{
namespace util
{
    template<typename T, typename U>
    bool is_type(U& u) noexcept
    {
        return is_type_ptr(&u);
    }

    template<typename T, typename U>
    bool is_type_ptr(U* u) noexcept
    {
        return dynamic_cast<T*>(u) != nullptr;
    }

    template<typename T, typename U>
    bool is_type(const U& u) noexcept
    {
        return is_type_ptr(&u);
    }

    template<typename T, typename U>
    bool is_type_ptr(const U* u) noexcept
    {
        return dynamic_cast<const T*>(u) != nullptr;
    }
}
}
