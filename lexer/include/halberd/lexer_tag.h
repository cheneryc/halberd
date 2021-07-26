#pragma once

#include <cstdint> // uint8_t


namespace halberd
{
namespace lexer
{
    enum class lexer_tag : uint8_t
    {
        none = 0U,
        accept_identifier       = 0x1,
        accept_literal_fraction = 0x2,
        accept_literal_integer  = 0x4
    };
}
}
