#pragma once

// halberd::lexer
#include <halberd/symbol.h>

// halberd::syntax
#include <halberd/expression.h>

#include <memory> // std::unique_ptr
#include <tuple> // std::tuple
#include <vector> // std::vector


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_operator_unary_postfix
    {
        std::unique_ptr<syntax::expression> operator()(std::tuple<std::unique_ptr<syntax::expression>, std::vector<lexer::symbol>> value) const;
    };
}
}
}
