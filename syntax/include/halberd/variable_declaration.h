#pragma once

#include "node.h"

#include <string>


namespace halberd
{
namespace syntax
{
    class variable_declaration : public node
    {
    public:
        variable_declaration(std::string name);

        const std::string name;
    };
}
}
