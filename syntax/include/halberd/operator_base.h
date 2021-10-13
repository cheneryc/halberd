#pragma once

#include "expression.h"
#include "type.h"

// halberd::util
#include <halberd/util/optional.h>


namespace halberd
{
namespace syntax
{
    class operator_base : public expression
    {
    public:
        type get_type() const override;

        void set_type(type t);

    private:
        util::optional<type> _type;
    };
}
}
