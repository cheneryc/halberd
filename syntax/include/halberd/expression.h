#pragma once

#include "node.h"

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple


namespace halberd
{
namespace syntax
{
    class expression : public node
    {
    };

    struct expression_transform
    {
        template<typename T>
        std::unique_ptr<expression> operator()(std::tuple<T> t) const
        {
            return std::make_unique<expression>();
        }
    };
}
}
