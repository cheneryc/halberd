#pragma once

#include "operator_base.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    // Forward declarations
    class expression;
    class visitor;
    class const_visitor;

    enum class operator_unary_postfix_id
    {
        increment,
        decrement,
    };

    class operator_unary_postfix : public operator_base
    {
    public:
        operator_unary_postfix(operator_unary_postfix_id operator_id, std::unique_ptr<expression> operand);

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        void reset_operand(std::unique_ptr<expression> exp);

        expression& get_operand();
        const expression& get_operand() const;

        const operator_unary_postfix_id operator_id;

    private:
        std::unique_ptr<expression> _operand;
    };
}
}
