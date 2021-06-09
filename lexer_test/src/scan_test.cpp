#include <halberd/scan.h>

#include <halberd/lexer.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::lexer;
}

TEST(scan, scan_empty_string)
{
    const auto tokens = ns::scan(ns::get_smv_union(), "");

    ASSERT_TRUE(tokens.empty());
}

TEST(scan, scan_whitespace)
{
    {
        const auto tokens = ns::scan(ns::get_smv_union(), " ");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "  ");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "\t");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "\t\t");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "\n");

        EXPECT_TRUE(tokens.empty());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "\n\n");

        EXPECT_TRUE(tokens.empty());
    }
}

TEST(scan, scan_symbol_add)
{
    const ns::symbol expected_symbol = ns::symbol::op_add;

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "+");

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), " +"); // leading whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "+ "); // trailing whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }
}

TEST(scan, scan_symbol_increment)
{
    const ns::symbol expected_symbol = ns::symbol::op_increment;

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "++");

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), " ++"); // leading whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "++ "); // trailing whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_EQ(expected_symbol, dynamic_cast<const ns::token_symbol&>(*tokens.front())._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "+ +"); // separating whitespace should not be ignored

        ASSERT_EQ(2U, tokens.size());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[0U])._symbol);
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1U])._symbol);
    }
}

TEST(scan, scan_identifier_character)
{
    const char expected_id[] = "a";

    {
        const auto tokens = ns::scan(ns::get_smv_union(), expected_id);

        ASSERT_EQ(1U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), " a"); // leading whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "a "); // trailing whitespace should be ignored

        ASSERT_EQ(1U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
    }
}

TEST(scan, scan_identifier_string)
{
    const char expected_id[] = "my_identifier";

    const auto tokens = ns::scan(ns::get_smv_union(), expected_id);

    ASSERT_EQ(1U, tokens.size());
    ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens.front())._identifier.c_str());
}

TEST(scan, scan_identifier_symbol)
{
    const char expected_id[] = "a";

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "a+");

        ASSERT_EQ(2U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "a +"); // whitespace should be ignored

        ASSERT_EQ(2U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "a + "); // whitespace should be ignored

        ASSERT_EQ(2U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
    }
}

TEST(scan, scan_identifier_symbol_identifier)
{
    const char expected_id1[] = "a";
    const char expected_id2[] = "b";

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "a=b");

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id1, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_assign, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_STREQ(expected_id2, dynamic_cast<const ns::token_identifier&>(*tokens[2])._identifier.c_str());
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "a = b"); // whitespace should be ignored

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id1, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_assign, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_STREQ(expected_id2, dynamic_cast<const ns::token_identifier&>(*tokens[2])._identifier.c_str());
    }
}

TEST(scan, scan_identifier_symbol_fractional)
{
    const char expected_id[] = "f";

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "f+1.0");

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_EQ(1.0f, dynamic_cast<const ns::token_literal_fractional&>(*tokens[2])._value);
    }

    {
        const auto tokens = ns::scan(ns::get_smv_union(), "f + 1.0"); // whitespace should be ignored

        ASSERT_EQ(3U, tokens.size());
        ASSERT_STREQ(expected_id, dynamic_cast<const ns::token_identifier&>(*tokens[0])._identifier.c_str());
        ASSERT_EQ(ns::symbol::op_add, dynamic_cast<const ns::token_symbol&>(*tokens[1])._symbol);
        ASSERT_EQ(1.0f, dynamic_cast<const ns::token_literal_fractional&>(*tokens[2])._value);
    }
}

TEST(scan, scan_error_invalid_identifier)
{
    ASSERT_THROW(ns::scan(ns::get_smv_union(), "0b"), std::exception); // identifier cannot begin with a numeric character
}
