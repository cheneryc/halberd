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

    enum class operator_assignment_id
    {
        assignment,
        compound_addition,
        compound_subtraction,
        compound_multiplication,
        compound_division,
        compound_remainder
    };

    class operator_assignment : public operator_base
    {
    public:
        operator_assignment(operator_assignment_id operator_id, std::unique_ptr<expression> operand_lhs, std::unique_ptr<expression> operand_rhs);

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        expression& get_operand_lhs();
        const expression& get_operand_lhs() const;

        expression& get_operand_rhs();
        const expression& get_operand_rhs() const;

        const operator_assignment_id operator_id;

    private:
        std::unique_ptr<expression> _operand_lhs;
        std::unique_ptr<expression> _operand_rhs;
    };
}
}
