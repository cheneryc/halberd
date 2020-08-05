#pragma once

#include "lexer_tag.h"
#include "state_machine_runner.h"
#include "token.h"

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace lexer
{
    std::vector<std::unique_ptr<token>> scan(state_machine_runner<char, lexer_tag> smr, const char* src);
}
}
