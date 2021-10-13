#include <halberd/literal.h>
#include <halberd/visitor.h>
#include <halberd/const_visitor.h>

#include <stdexcept> // std::logic_error


namespace
{
    namespace ns = halberd::syntax;
}

ns::literal::literal(int i) noexcept : id(literal_id::integer), _value_i(i)
{
}

ns::literal::literal(float f) noexcept : id(literal_id::fractional), _value_f(f)
{
}

void ns::literal::accept(visitor& v)
{
    v.visit(*this);
}

void ns::literal::accept(const_visitor& cv) const
{
    cv.visit(*this);
}

ns::type ns::literal::get_type() const
{
    type_id tid;

    switch (id)
    {
        case literal_id::integer:
            tid = type_id::type_i32;
            break;
        case literal_id::fractional:
            tid = type_id::type_f32;
            break;
        default:
            throw std::logic_error("unexpected value for halberd::syntax::literal_id enumeration");
    }

    return { tid };
}

int ns::literal::get_integer() const
{
    if (id != literal_id::integer)
    {
        throw std::logic_error("invalid state - syntax::literal value type is not literal_id::integer");
    }

    return _value_i;
}

float ns::literal::get_fractional() const
{
    if (id != literal_id::fractional)
    {
        throw std::logic_error("invalid state - syntax::literal value type is not literal_id::fractional");
    }

    return _value_f;
}
