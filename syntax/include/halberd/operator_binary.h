#pragma once

#include "expression.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    enum class operator_binary_id
    {
        addition,
        subtraction,
        multiplication,
        division,
        remainder
    };

    class operator_binary : public expression
    {
    public:
        operator_binary(operator_binary_id operator_id, std::unique_ptr<expression> operand_lhs, std::unique_ptr<expression> operand_rhs);

        const expression& get_operand_lhs() const;
        const expression& get_operand_rhs() const;

        const operator_binary_id operator_id;

    private:
        std::unique_ptr<expression> _operand_lhs;
        std::unique_ptr<expression> _operand_rhs;
    };
}
}
