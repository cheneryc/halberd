#pragma once

#include "rule.h"

// halberd::lexer
#include <halberd/token.h>

// halberd::parser
#include <halberd/parse_result.h>

// halberd::syntax
#include <halberd/node.h>

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace compiler
{
    //TODO: return type should be parse_result<std::unique_ptr<syntax::tree>>
    bool compile(const char* src);
    bool compile(std::vector<std::unique_ptr<lexer::token>> tokens);

    parser::parse_result<std::unique_ptr<syntax::node>> compile_rule(rule r, const char* src);
    parser::parse_result<std::unique_ptr<syntax::node>> compile_rule(rule r, std::vector<std::unique_ptr<lexer::token>> tokens);
}
}
