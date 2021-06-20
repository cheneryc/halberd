#pragma once

#include "expression.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    enum class operator_unary_prefix_id
    {
        increment,
        decrement,
    };

    class operator_unary_prefix : public expression
    {
    public:
        operator_unary_prefix(operator_unary_prefix_id operator_id, std::unique_ptr<expression> operand);

        const expression& get_operand() const;

        const operator_unary_prefix_id operator_id;

    private:
        std::unique_ptr<expression> _operand;
    };
}
}
