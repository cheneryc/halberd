#pragma once

#include "node.h"

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple


namespace halberd
{
namespace syntax
{
    class statement : public node
    {
    protected:
        statement() = default;
    };
}
}
