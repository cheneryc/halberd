#pragma once

#include "rule.h"

#include <halberd/token.h>

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace compiler
{
    //TODO: return type should be parse_result<std::unique_ptr<syntax::tree>>
    bool compile(const char* src);
    bool compile(std::vector<std::unique_ptr<lexer::token>> tokens);

    //TODO: return type should be parse_result<std::unique_ptr<syntax::node>>
    bool compile_rule(rule r, const char* src);
    bool compile_rule(rule r, std::vector<std::unique_ptr<lexer::token>> tokens);
}
}
