#pragma once

#include "node.h"
#include "type.h"


namespace halberd
{
namespace syntax
{
    class expression : public node
    {
    public:
        virtual type get_type() const = 0;

    protected:
        expression() = default;
    };
}
}
