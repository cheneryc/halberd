#include <halberd/expression_multiplicative_transform.h>
#include <halberd/operator_binary.h>

#include <stdexcept> // std::invalid_argument


namespace
{
    namespace ns = halberd::compiler;
}

std::unique_ptr<halberd::syntax::expression> ns::expression_multiplicative_transform::operator()(std::tuple<std::unique_ptr<syntax::expression>, std::vector<std::tuple<lexer::symbol, std::unique_ptr<syntax::expression>>>> value) const
{
    std::unique_ptr<syntax::expression> exp_operator;
    std::vector<std::tuple<lexer::symbol, std::unique_ptr<syntax::expression>>> sym_exp_pairs;

    std::tie(exp_operator, sym_exp_pairs) = std::move(value);

    if (!exp_operator)
    {
        throw std::invalid_argument("halberd::syntax::expression_multiplicative_transform: expression argument cannot be null");
    }

    for (auto& sym_exp_pair : sym_exp_pairs)
    {
        lexer::symbol symbol;
        std::unique_ptr<syntax::expression> exp_operand;

        std::tie(symbol, exp_operand) = std::move(sym_exp_pair);

        syntax::operator_binary_id operator_id;

        switch (symbol)
        {
            case lexer::symbol::sign_plus:
                operator_id = syntax::operator_binary_id::addition;
                break;
            case lexer::symbol::sign_minus:
                operator_id = syntax::operator_binary_id::subtraction;
                break;
            case lexer::symbol::asterisk:
                operator_id = syntax::operator_binary_id::multiplication;
                break;
            case lexer::symbol::slash:
                operator_id = syntax::operator_binary_id::division;
                break;
            case lexer::symbol::sign_percent:
                operator_id = syntax::operator_binary_id::remainder;
                break;
            default:
                throw std::invalid_argument("invalid symbol for halberd::syntax::expression_multiplicative_transform");
        }

        // Construct a new operator_binary expression with
        // the current operator as the lhs operand expression
        exp_operator = std::make_unique<syntax::operator_binary>(operator_id, std::move(exp_operator), std::move(exp_operand));
    }

    return exp_operator;
}
