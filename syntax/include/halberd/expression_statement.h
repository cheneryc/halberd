#pragma once

#include "statement.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class expression;
    class visitor;
    class const_visitor;

    class expression_statement : public statement
    {
    public:
        expression_statement(std::unique_ptr<expression> exp) noexcept;

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        void reset_expression(std::unique_ptr<expression> exp);

        expression& get_expression();
        const expression& get_expression() const;

    private:
        std::unique_ptr<expression> _exp;
    };
}
}
