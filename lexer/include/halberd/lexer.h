#pragma once

#include "state_machine_array.h"


namespace halberd
{
namespace lexer
{
    state_machine_view<char> get_smv_identifier() noexcept;
    state_machine_view<char> get_smv_fractional_literal() noexcept;
    state_machine_view<char> get_smv_union() noexcept;
}
}
