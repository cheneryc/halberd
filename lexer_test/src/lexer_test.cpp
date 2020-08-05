#include <halberd/lexer.h>

#include <halberd/state_machine_array.h>
#include <halberd/state_machine_runner.h> // try_transition

#include <halberd/util/string.h> // halberd::util::length

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::lexer;

    template<typename TSym, typename TTag, typename It>
    constexpr bool is_accepted(const ns::state_machine_view<TSym, TTag>& smv, It it_symbol, const It it_end)
    {
        auto idx_state = smv.idx_start;

        while ((it_symbol != it_end) && ns::try_transition(smv, *it_symbol, idx_state))
        {
            ++it_symbol;
        }

        //TODO: check if asserts can be used in constexpr functions. Replace with static_assert?
        //assert(((it_symbol == it_end) && (idx_state != state::state_index_invalid))
        //    || ((it_symbol != it_end) && (idx_state == state::state_index_invalid)));

        return (it_symbol == it_end) && smv[idx_state].is_accept_state;
    }

    template<typename TSym, typename TTag, std::size_t N>
    constexpr bool is_accepted(const ns::state_machine_view<TSym, TTag>& smv, const TSym (&symbols)[N])
    {
        return is_accepted(smv, symbols, symbols + halberd::util::length(symbols));
    }
}

TEST(lexer, state_machine_view_identifier_accept)
{
    const auto smv_identifier = ns::get_smv_identifier();

    EXPECT_TRUE(::is_accepted(smv_identifier, "_"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "a"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "A"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "_0"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "_a"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "_A"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "__"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "aa"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "AA"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "A0"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "AA00"));
    EXPECT_TRUE(::is_accepted(smv_identifier, "a_b"));
}

TEST(lexer, state_machine_view_identifier_reject)
{
    const auto smv_identifier = ns::get_smv_identifier();

    EXPECT_FALSE(::is_accepted(smv_identifier, ""));
    EXPECT_FALSE(::is_accepted(smv_identifier, "0"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "00"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "0_"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "0a"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "0A"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "00__"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "00aa"));
    EXPECT_FALSE(::is_accepted(smv_identifier, "00AA"));
}

TEST(lexer, state_machine_view_fractional_literal_accept)
{
    const auto smv_fractional = ns::get_smv_fractional_literal();

    EXPECT_TRUE(::is_accepted(smv_fractional, "0."));
    EXPECT_TRUE(::is_accepted(smv_fractional, ".0"));
    EXPECT_TRUE(::is_accepted(smv_fractional, "0.0"));
    EXPECT_TRUE(::is_accepted(smv_fractional, "0123456789.0123456789"));
}

TEST(lexer, state_machine_view_fractional_literal_reject)
{
    const auto smv_fractional = ns::get_smv_fractional_literal();

    EXPECT_FALSE(::is_accepted(smv_fractional, ""));
    EXPECT_FALSE(::is_accepted(smv_fractional, "0"));
    EXPECT_FALSE(::is_accepted(smv_fractional, "0.a"));
    EXPECT_FALSE(::is_accepted(smv_fractional, "a.0"));
    EXPECT_FALSE(::is_accepted(smv_fractional, "0123456789"));
}
