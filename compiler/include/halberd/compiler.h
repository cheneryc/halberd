#pragma once

#include <halberd/token.h>

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace compiler
{
    bool compile(const char* src);
    bool compile(std::vector<std::unique_ptr<halberd::lexer::token>> tokens);
}
}
