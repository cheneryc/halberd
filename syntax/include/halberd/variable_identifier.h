#pragma once

#include "expression.h"
#include "type.h"


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class variable_declaration;
    class visitor;
    class const_visitor;

    class variable_identifier : public expression
    {
    public:
        variable_identifier(variable_declaration* dec);

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        type get_type() const override;

        const variable_declaration& get_variable_declaration() const noexcept;

    private:
        variable_declaration* const _declaration;
    };
}
}
