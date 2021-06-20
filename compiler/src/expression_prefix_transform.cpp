#include <halberd/expression_prefix_transform.h>
#include <halberd/operator_unary_prefix.h>

#include <stdexcept> // std::invalid_argument


namespace
{
    namespace ns = halberd::compiler;
}

std::unique_ptr<halberd::syntax::expression> ns::expression_prefix_transform::operator()(std::tuple<std::vector<lexer::symbol>, std::unique_ptr<syntax::expression>> value) const
{
    std::vector<lexer::symbol> symbols;
    std::unique_ptr<syntax::expression> exp_operator;

    std::tie(symbols, exp_operator) = std::move(value);

    if (!exp_operator)
    {
        throw std::invalid_argument("halberd::syntax::expression_prefix_transform: expression argument cannot be null");
    }

    for (const auto& symbol : symbols)
    {
        syntax::operator_unary_prefix_id operator_id;

        switch (symbol)
        {
            case lexer::symbol::op_increment:
                operator_id = syntax::operator_unary_prefix_id::increment;
                break;
            case lexer::symbol::op_decrement:
                operator_id = syntax::operator_unary_prefix_id::decrement;
                break;
            default:
                throw std::invalid_argument("invalid symbol for halberd::syntax::expression_prefix_transform");
        }

        // Construct a new operator_unary_prefix expression
        // with the current operator as the operand expression
        exp_operator = std::make_unique<syntax::operator_unary_prefix>(operator_id, std::move(exp_operator));
    }

    return exp_operator;
}
