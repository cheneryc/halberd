#include <halberd/operator_base.h>


namespace
{
    namespace ns = halberd::syntax;
}

ns::type ns::operator_base::get_type() const
{
    return _type.get();
}

void ns::operator_base::set_type(ns::type t)
{
    _type = t;
}
