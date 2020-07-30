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

TEST(lexer, scan_empty_string)
{
    const auto tokens = ns::scan("");

    ASSERT_TRUE(tokens.empty());
}

TEST(lexer, scan_whitespace)
{
    {
        const auto tokens = ns::scan(" ");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan("  ");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan("\t");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan("\t\t");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan("\n");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan("\n\n");

        EXPECT_TRUE(tokens.empty());
    }
}

TEST(lexer, scan_symbol_add)
{
    const ns::symbol expected_symbol = ns::symbol::op_add;

    {
        const auto tokens = ns::scan("+");

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan(" +"); // leading whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan("+ "); // trailing whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }
}

TEST(lexer, scan_identifier_character)
{
    const char expected_id[] = "a";

    {
        const auto tokens = ns::scan(expected_id);

        ASSERT_EQ(1U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
    }

    {
        const auto tokens = ns::scan(" a"); // leading whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
    }

    {
        const auto tokens = ns::scan("a "); // trailing whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
    }
}

TEST(lexer, scan_identifier_string)
{
    const char expected_id[] = "my_identifier";

    const auto tokens = ns::scan(expected_id);

    ASSERT_EQ(1U, tokens.size());
    ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
}

TEST(lexer, scan_identifier_symbol)
{
    const char expected_id[] = "a";

    {
        const auto tokens = ns::scan("a+");

        ASSERT_EQ(2U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
    }

    {
        const auto tokens = ns::scan("a +"); // whitespace should be ignored

        ASSERT_EQ(2U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
    }

    {
        const auto tokens = ns::scan("a + "); // whitespace should be ignored

        ASSERT_EQ(2U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
    }
}

TEST(lexer, scan_identifier_symbol_identifier)
{
    const char expected_id1[] = "a";
    const char expected_id2[] = "b";

    {
        const auto tokens = ns::scan("a=b");

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id1, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_assign, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_STREQ(expected_id2, dynamic_cast<const ns::token_identifier&>(*tokens[2])._identifier.c_str());
    }

    {
        const auto tokens = ns::scan("a = b"); // whitespace should be ignored

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id1, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_assign, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_STREQ(expected_id2, dynamic_cast<const ns::token_identifier&>(*tokens[2])._identifier.c_str());
    }
}

TEST(lexer, scan_identifier_symbol_fractional)
{
    const char expected_id[] = "f";

    {
        const auto tokens = ns::scan("f+1.0");

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_EQ(1.0f, dynamic_cast<const ns::token_literal_fractional&>(*tokens[2])._value);
    }

    {
        const auto tokens = ns::scan("f + 1.0"); // whitespace should be ignored

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_EQ(1.0f, dynamic_cast<const ns::token_literal_fractional&>(*tokens[2])._value);
    }
}

TEST(lexer, scan_error_invalid_identifier)
{
    ASSERT_THROW(ns::scan("0b"), std::exception); // identifier cannot begin with a numeric character
}
