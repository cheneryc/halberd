#pragma once

#include "declaration.h"

#include <string> // std::string


namespace halberd
{
namespace syntax
{
    class variable_declaration : public declaration
    {
    public:
        variable_declaration(std::string name) noexcept;
    };
}
}
