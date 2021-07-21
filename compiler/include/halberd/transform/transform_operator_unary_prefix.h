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
    struct transform_operator_unary_prefix
    {
        std::unique_ptr<syntax::expression> operator()(std::tuple<std::vector<lexer::symbol>, std::unique_ptr<syntax::expression>> value) const;
    };
}
}
}
