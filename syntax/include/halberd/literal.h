#pragma once

#include "expression.h"


namespace halberd
{
namespace syntax
{
    enum class literal_id
    {
        integer,
        fractional
    };

    class literal : public expression
    {
    public:
        literal(int i) noexcept;
        literal(float f) noexcept;

        int get_integer() const;
        float get_fractional() const;

        const literal_id id;

    private:
        union
        {
            int   _value_i;
            float _value_f;
        };
    };
}
}
