#pragma once

#include "declaration_statement.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class variable_declaration;
    class visitor;
    class const_visitor;

    class variable_declaration_statement : public declaration_statement
    {
    public:
        variable_declaration_statement(std::unique_ptr<variable_declaration> vd) noexcept;

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        variable_declaration& get_variable_declaration();
        const variable_declaration& get_variable_declaration() const;
    };
}
}
