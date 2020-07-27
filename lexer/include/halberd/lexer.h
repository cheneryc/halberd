#pragma once

#include "state_machine_array.h"
#include "token.h"

#include <memory> // std::unique_ptr
#include <vector> // std::vector


namespace halberd
{
namespace lexer
{
    state_machine_view<char> get_smv_identifier() noexcept;
    state_machine_view<char> get_smv_fractional_literal() noexcept;
    state_machine_view<char> get_smv_union() noexcept;

    std::vector<std::unique_ptr<token>> scan(const char* str);
}
}
