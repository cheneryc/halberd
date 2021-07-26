#include <halberd/literal.h>

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
