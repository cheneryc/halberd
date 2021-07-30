#pragma once

#include "statement.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class declaration;

    class declaration_statement : public statement
    {
    protected:
        declaration_statement(std::unique_ptr<declaration> dec) noexcept;

    private:
        std::unique_ptr<declaration> _dec;
    };
}
}
