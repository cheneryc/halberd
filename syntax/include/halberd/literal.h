#pragma once

#include "expression.h"
#include "type.h"


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class visitor;
    class const_visitor;

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

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        type get_type() const override;

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
