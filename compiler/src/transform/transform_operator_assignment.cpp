#include <halberd/transform/transform_operator_assignment.h>

// halberd::syntax
#include <halberd/operator_assignment.h>

#include <stdexcept> // std::invalid_argument


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::expression> ns::transform_operator_assignment::operator()(std::tuple<std::unique_ptr<syntax::expression>, lexer::symbol, std::unique_ptr<syntax::expression>> value) const
{
    lexer::symbol symbol;
    std::unique_ptr<syntax::expression> exp_lhs; // unary expression
    std::unique_ptr<syntax::expression> exp_rhs; // assignment expression

    std::tie(exp_lhs, symbol, exp_rhs) = std::move(value);

    if (!exp_lhs)
    {
        throw std::invalid_argument("halberd::compiler::transform::transform_operator_assignment: lhs expression argument cannot be null");
    }

    if (!exp_rhs)
    {
        throw std::invalid_argument("halberd::compiler::transform::transform_operator_assignment: rhs expression argument cannot be null");
    }

    syntax::operator_assignment_id operator_id;

    switch (symbol)
    {
        case lexer::symbol::sign_equals:
            operator_id = syntax::operator_assignment_id::assignment;
            break;
        case lexer::symbol::assign_asterisk:
            operator_id = syntax::operator_assignment_id::compound_multiplication;
            break;
        case lexer::symbol::assign_slash:
            operator_id = syntax::operator_assignment_id::compound_division;
            break;
        case lexer::symbol::assign_percent:
            operator_id = syntax::operator_assignment_id::compound_remainder;
            break;
        case lexer::symbol::assign_plus:
            operator_id = syntax::operator_assignment_id::compound_addition;
            break;
        case lexer::symbol::assign_minus:
            operator_id = syntax::operator_assignment_id::compound_subtraction;
            break;
        default:
            throw std::invalid_argument("invalid symbol for halberd::compiler::transform::transform_operator_assignment");
    }

    return std::make_unique<syntax::operator_assignment>(operator_id, std::move(exp_lhs), std::move(exp_rhs));
}
