#include <halberd/scanner.h>

#include <halberd/keyword.h>
#include <halberd/symbol.h>

#include <exception> // std::exception
#include <string> // std::basic_string

#include <cctype> // std::isspace


namespace
{
    namespace ns = halberd::lexer;

    std::unique_ptr<ns::token> tokenize_identifier(std::basic_string<char>&& str_token)
    {
        const auto result_keyword = ns::to_keyword(str_token.c_str(), str_token.length());

        if (result_keyword.second)
        {
            return std::make_unique<ns::token_identifier_reserved>(result_keyword.first);
        }
        else
        {
            return std::make_unique<ns::token_identifier>(std::move(str_token));
        }
    }

    std::unique_ptr<ns::token> tokenize(std::basic_string<char>&& str_token, ns::lexer_tag tag)
    {
        std::unique_ptr<ns::token> token;

        switch (tag)
        {
            case ns::lexer_tag::accept_identifier:
                token = ::tokenize_identifier(std::move(str_token));
                break;
            case ns::lexer_tag::accept_literal_fraction:
                token = std::make_unique<ns::token_literal_fractional>(str_token);
                break;
            case ns::lexer_tag::accept_literal_integer:
                token = std::make_unique<ns::token_literal_integer>(str_token);
                break;
            default:
                throw std::exception();
        }

        return token;
    }

    std::unique_ptr<ns::token> tokenize(std::basic_ostringstream<char>& oss, ns::lexer_tag tag)
    {
        std::basic_string<char> str_token = oss.str();

        if (str_token.empty())
        {
            throw std::exception();
        }

        // Reset the stringstream
        oss.str(std::basic_string<char>());
        oss.clear();

        return ::tokenize(std::move(str_token), tag);
    }
}

ns::scanner::scanner(ns::state_machine_runner<char, ns::lexer_tag> smr, const char* src) : _smr(smr),
    _token_extra(),
    _ss_input(src),
    _ss_token(),
    _is_eof(false)
{
    _ss_input >> std::noskipws;
}

std::unique_ptr<ns::token> ns::scanner::scan()
{
    std::unique_ptr<token> token;

    if (_is_eof)
    {
        throw std::exception();
    }

    if (_token_extra)
    {
        token = std::move(_token_extra);
    }
    else
    {
        char ch;

        while (!token && _ss_input >> ch)
        {
            // Get the current state's properties before calling try_transition,
            // in case the state machine transitions to an invalid state.
            const lexer_tag tag = _smr.get_state_tag();

            const bool is_state_start = _smr.is_state_start();
            const bool is_state_accepting = _smr.is_state_accepting();

            if (_smr.try_transition(ch))
            {
                _ss_token << ch;
            }
            else
            {
                if (is_state_accepting)
                {
                    token = ::tokenize(_ss_token, tag);
                }
                else if (!is_state_start)
                {
                    throw std::exception();
                }

                const auto result_symbol = ns::to_symbol(ch, _ss_input);

                if (result_symbol.second)
                {
                    (!token ? token : _token_extra) = std::make_unique<token_symbol>(result_symbol.first);
                }
                else if (!std::isspace(static_cast<unsigned char>(ch)))
                {
                    throw std::exception();
                }

                _smr.reset();
            }
        }

        if (!token)
        {
            if (_smr.is_state_accepting())
            {
                token = ::tokenize(_ss_token, _smr.get_state_tag());
            }
            else if (!_smr.is_state_start())
            {
                throw std::exception();
            }

            _smr.reset();
        }

        if (!token)
        {
            _is_eof = true;
        }
    }

    return token;
}
