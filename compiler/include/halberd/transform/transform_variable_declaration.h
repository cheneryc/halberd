#pragma once

// halberd::lexer
#include <halberd/keyword.h>

// halberd::syntax
#include <halberd/variable_declaration.h>

#include <memory> // std::unique_ptr
#include <string> // std::string
#include <tuple> // std::tuple


namespace halberd
{
namespace compiler
{
namespace transform
{
    struct transform_variable_declaration
    {
        std::unique_ptr<syntax::variable_declaration> operator()(std::tuple<lexer::keyword, std::string> value) const;
    };
}
}
}
