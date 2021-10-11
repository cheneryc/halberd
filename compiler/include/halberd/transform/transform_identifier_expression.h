#pragma once

// halberd::syntax
#include <halberd/expression.h>

#include <memory> // std::unique_ptr
#include <string> // std::string
#include <tuple> // std::tuple


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_identifier_expression
    {
        std::unique_ptr<syntax::expression> operator()(std::tuple<std::string> value) const;
    };
}
}
}
