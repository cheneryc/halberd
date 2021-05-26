#pragma once

#include "expression.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    enum class operator_unary_postfix_id
    {
        increment,
        decrement,
    };

    class operator_unary_postfix : public expression
    {
    public:
        operator_unary_postfix(operator_unary_postfix_id operator_id, std::unique_ptr<expression> operand);

        const operator_unary_postfix_id operator_id;

    private:
        std::unique_ptr<expression> _operand;
    };
}
}
