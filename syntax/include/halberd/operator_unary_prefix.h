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

    enum class operator_unary_prefix_id
    {
        increment,
        decrement,
    };

    class operator_unary_prefix : public operator_base
    {
    public:
        operator_unary_prefix(operator_unary_prefix_id operator_id, std::unique_ptr<expression> operand);

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        expression& get_operand();
        const expression& get_operand() const;

        const operator_unary_prefix_id operator_id;

    private:
        std::unique_ptr<expression> _operand;
    };
}
}
