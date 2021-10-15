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

    enum class operator_binary_id
    {
        addition,
        subtraction,
        multiplication,
        division,
        remainder
    };

    class operator_binary : public operator_base
    {
    public:
        operator_binary(operator_binary_id operator_id, std::unique_ptr<expression> operand_lhs, std::unique_ptr<expression> operand_rhs);

        void accept(visitor& v) override;
        void accept(const_visitor& cv) const override;

        void reset_operand_lhs(std::unique_ptr<expression> exp);
        void reset_operand_rhs(std::unique_ptr<expression> exp);

        expression& get_operand_lhs();
        const expression& get_operand_lhs() const;

        expression& get_operand_rhs();
        const expression& get_operand_rhs() const;

        const operator_binary_id operator_id;

    private:
        std::unique_ptr<expression> _operand_lhs;
        std::unique_ptr<expression> _operand_rhs;
    };
}
}
