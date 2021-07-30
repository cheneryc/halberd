#pragma once

#include "statement.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class expression;

    class expression_statement : public statement
    {
    public:
        expression_statement(std::unique_ptr<expression> exp) noexcept;

    private:
        std::unique_ptr<expression> _exp;
    };
}
}
