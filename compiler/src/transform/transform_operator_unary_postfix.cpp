#include <halberd/transform/transform_operator_unary_postfix.h>

// halberd::syntax
#include <halberd/operator_unary_postfix.h>

#include <stdexcept> // std::invalid_argument


namespace
{
    namespace ns = halberd::compiler::transform;
}

std::unique_ptr<halberd::syntax::expression> ns::transform_operator_unary_postfix::operator()(std::tuple<std::unique_ptr<syntax::expression>, std::vector<lexer::symbol>> value) const
{
    std::vector<lexer::symbol> symbols;
    std::unique_ptr<syntax::expression> exp_operator;

    std::tie(exp_operator, symbols) = std::move(value);

    if (!exp_operator)
    {
        throw std::invalid_argument("halberd::compiler::transform::transform_operator_unary_postfix: expression argument cannot be null");
    }

    for (const auto& symbol : symbols)
    {
        syntax::operator_unary_postfix_id operator_id;

        switch (symbol)
        {
            case lexer::symbol::op_increment:
                operator_id = syntax::operator_unary_postfix_id::increment;
                break;
            case lexer::symbol::op_decrement:
                operator_id = syntax::operator_unary_postfix_id::decrement;
                break;
            default:
                throw std::invalid_argument("invalid symbol for halberd::compiler::transform::transform_operator_unary_postfix");
        }

        // Construct a new operator_unary_postfix expression
        // with the current operator as the operand expression
        exp_operator = std::make_unique<syntax::operator_unary_postfix>(operator_id, std::move(exp_operator));
    }

    return exp_operator;
}
