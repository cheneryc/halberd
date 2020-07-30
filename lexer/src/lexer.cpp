#include <halberd/lexer.h>

#include <halberd/keyword.h>
#include <halberd/state_machine.h>
#include <halberd/state_machine_array.h>
#include <halberd/state_machine_runner.h>
#include <halberd/state_machine_operators.h>
#include <halberd/symbol.h>
#include <halberd/symbol_set.h>
#include <halberd/symbol_set_operators.h>

#include <sstream> // std::basic_ostringstream


namespace
{
    namespace ns = halberd::lexer;

    template<typename TSym>
    constexpr ns::basic_symbol_range<TSym, '0', '9'> digit_symbol_range;

    template<typename TSym>
    constexpr auto get_sm_identifier() noexcept
    {
        constexpr auto nondigit_symbol_set =
            ns::basic_symbol_range_v<TSym, 'a', 'z'> |
            ns::basic_symbol_range_v<TSym, 'A', 'Z'> |
            ns::basic_symbol_v<TSym, '_'>;

        constexpr auto s_end =
            ns::state_v<TSym, 0U, true, ns::lexer_tag, ns::lexer_tag::accept_identifier>
                + ns::make_transition_self(nondigit_symbol_set)
                + ns::make_transition_self(digit_symbol_range<TSym>);

        constexpr auto s_start =
            ns::state_v<TSym, ns::next_index(s_end), false, ns::lexer_tag>
                + ns::make_transition(nondigit_symbol_set, ns::get_index_tag(s_end));

        constexpr auto sm =
            ns::state_machine_v<TSym, ns::get_index(s_start), ns::lexer_tag>
                + s_end
                + s_start;

        return sm;
    }

    template<typename TSym>
    constexpr auto get_sm_fractional_literal() noexcept
    {
        constexpr auto period_symbol = ns::basic_symbol_v<TSym, '.'>;

        constexpr auto s_end =
            ns::state_v<TSym, 0U, true, ns::lexer_tag, ns::lexer_tag::accept_literal_fraction>
                + ns::make_transition_self(digit_symbol_range<TSym>);

        constexpr auto s1 =
            ns::state_v<TSym, ns::next_index(s_end), false, ns::lexer_tag>
                + ns::make_transition(digit_symbol_range<TSym>, ns::get_index_tag(s_end));
                
        constexpr auto s2 =
            ns::state_v<TSym, ns::next_index(s1), false, ns::lexer_tag>
                + ns::make_transition_self(digit_symbol_range<TSym>)
                + ns::make_transition(period_symbol, ns::get_index_tag(s_end));

        constexpr auto s_start =
            ns::state_v<TSym, ns::next_index(s2), false, ns::lexer_tag>
                + ns::make_transition(period_symbol, ns::get_index_tag(s1))
                + ns::make_transition(digit_symbol_range<TSym>, ns::get_index_tag(s2));

        constexpr auto sm =
            ns::state_machine_v<TSym, ns::get_index(s_start), ns::lexer_tag>
                + s_end
                + s1
                + s2
                + s_start;

        return sm;
    }

    std::unique_ptr<ns::token> tokenize_identifier(std::string&& str_token)
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

    std::unique_ptr<ns::token> tokenize(std::string&& str_token, ns::lexer_tag tag)
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
            default:
                throw std::exception();
        }

        return token;
    }

    std::unique_ptr<ns::token> tokenize(std::basic_ostringstream<char>& oss, ns::lexer_tag tag)
    {
        std::unique_ptr<ns::token> token;

        //TODO: is this check necessary? check the return value of str() instead?
        if (oss.tellp())
        {
            std::string str_token = oss.str();

            // Reset the stringstream
            oss.str(std::string());
            oss.clear();

            token = ::tokenize(std::move(str_token), tag);
        }

        return token;
    }
}

namespace halberd
{
namespace lexer
{
    constexpr lexer_tag operator|(lexer_tag tag1, lexer_tag tag2) noexcept
    {
        const auto tag_or = static_cast<std::underlying_type_t<lexer_tag>>(tag1) |
                            static_cast<std::underlying_type_t<lexer_tag>>(tag2);

        return static_cast<lexer_tag>(tag_or);
    }
}
}

ns::state_machine_view<char, ns::lexer_tag> ns::get_smv_identifier() noexcept
{
    constexpr auto sm = ::get_sm_identifier<char>();
    constexpr auto smv = ns::to_state_machine_view(sm);

    return smv;
}

ns::state_machine_view<char, ns::lexer_tag> ns::get_smv_fractional_literal() noexcept
{
    constexpr auto sm = ::get_sm_fractional_literal<char>();
    constexpr auto smv = ns::to_state_machine_view(sm);

    return smv;
}

ns::state_machine_view<char, ns::lexer_tag> ns::get_smv_union() noexcept
{
    constexpr auto sm_union =
        ::get_sm_identifier<char>() |
        ::get_sm_fractional_literal<char>();

    constexpr auto smv_union = ns::to_state_machine_view(sm_union);

    return smv_union;
}

std::vector<std::unique_ptr<ns::token>> ns::scan(const char* str)
{
    std::vector<std::unique_ptr<ns::token>> tokens;

    ns::state_machine_runner<char, ns::lexer_tag> smr_union(ns::get_smv_union());

    std::basic_istringstream<char> ss(str);
    std::basic_ostringstream<char> ss_token;

    //TODO: pass flag to ss_input constructor instead?
    ss >> std::noskipws;

    char ch;

    while (ss >> ch)
    {
        // Get the current state's properties before calling try_transition,
        // in case the state machine transitions to an invalid state.
        const lexer_tag tag = smr_union.get_state_tag();

        const bool is_start = smr_union.is_state_start();
        const bool is_accepting = smr_union.is_state_accepting();

        if (smr_union.try_transition(ch))
        {
            ss_token << ch;
        }
        else if (is_start || is_accepting)
        {
            const auto result_symbol = ns::to_symbol(ch);

            if (result_symbol.second)
            {
                if (auto token = ::tokenize(ss_token, tag))
                {
                    tokens.push_back(std::move(token));
                }

                tokens.push_back(std::make_unique<ns::token_symbol>(result_symbol.first));
            }
            else if (std::isspace(static_cast<unsigned char>(ch)))
            {
                if (auto token = ::tokenize(ss_token, tag))
                {
                    tokens.push_back(std::move(token));
                }
            }
            else
            {
                throw std::exception();
            }

            smr_union.reset();
        }
        else
        {
            throw std::exception();
        }
    }

    if (smr_union.is_state_accepting())
    {
        //TODO: throw an exception if the returned token is null? is that even possible if the state machine is in an accept state?
        if (auto token = ::tokenize(ss_token, smr_union.get_state_tag()))
        {
            tokens.push_back(std::move(token));
        }
    }
    else if (!smr_union.is_state_valid() ||
             !smr_union.is_state_start())
    {
        throw std::exception();
    }

    return tokens;
}
