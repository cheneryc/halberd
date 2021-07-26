#pragma once

#include "lexer_tag.h"
#include "state_machine_array.h"


namespace halberd
{
namespace lexer
{
    state_machine_view<char, lexer_tag> get_smv_identifier() noexcept;
    state_machine_view<char, lexer_tag> get_smv_fractional_literal() noexcept;
    state_machine_view<char, lexer_tag> get_smv_integer_literal() noexcept;
    state_machine_view<char, lexer_tag> get_smv_union() noexcept;
}
}
