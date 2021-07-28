#pragma once

#include "expression.h"

#include <memory> // std::unique_ptr


namespace halberd
{
namespace syntax
{
    enum class operator_assignment_id
    {
        assignment,
        compound_addition,
        compound_subtraction,
        compound_multiplication,
        compound_division,
        compound_remainder
    };

    class operator_assignment : public expression
    {
    public:
        operator_assignment(operator_assignment_id operator_id, std::unique_ptr<expression> operand_lhs, std::unique_ptr<expression> operand_rhs);

        const expression& get_operand_lhs() const;
        const expression& get_operand_rhs() const;

        const operator_assignment_id operator_id;

    private:
        std::unique_ptr<expression> _operand_lhs;
        std::unique_ptr<expression> _operand_rhs;
    };
}
}
