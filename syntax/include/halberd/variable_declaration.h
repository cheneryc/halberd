#pragma once

#include "declaration.h"

#include <string> // std::string


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class visitor;
    class const_visitor;

    class variable_declaration : public declaration
    {
    public:
        variable_declaration(std::string name) noexcept;

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;
    };
}
}
