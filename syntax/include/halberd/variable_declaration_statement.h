#pragma once

#include "declaration_statement.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class variable_declaration;

    class variable_declaration_statement : public declaration_statement
    {
    public:
        variable_declaration_statement(std::unique_ptr<variable_declaration> vd) noexcept;
    };
}
}
