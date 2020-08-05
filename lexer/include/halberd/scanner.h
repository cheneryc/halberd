#pragma once

#include "lexer_tag.h"
#include "state_machine_runner.h"
#include "token.h"

#include <memory> // std::unique_ptr
#include <sstream> // std::basic_istringstream, std::basic_ostringstream


namespace halberd
{
namespace lexer
{
    class scanner
    {
    public:
        scanner(state_machine_runner<char, lexer_tag> smr, const char* src);

        std::unique_ptr<token> scan();

    private:
        state_machine_runner<char, lexer_tag> _smr;

        std::unique_ptr<token> _token_extra;

        std::basic_istringstream<char> _ss_input;
        std::basic_ostringstream<char> _ss_token;

        bool _is_eof;
    };
}
}
