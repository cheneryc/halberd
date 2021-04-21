#pragma once

#include "node.h"

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple


namespace halberd
{
namespace syntax
{
    class variable_declaration : public node
    {
    };

    struct variable_declaration_transform
    {
        template<typename T>
        std::unique_ptr<variable_declaration> operator()(std::tuple<T, T, T> t) const
        {
            return {}; //TODO: implement this transform
        }
    };
}
}
