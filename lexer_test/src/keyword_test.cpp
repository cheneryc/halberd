#include <halberd/keyword.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::lexer;
}

TEST(keyword, to_string)
{
    EXPECT_STREQ("var", ns::to_string(ns::keyword::strict_var));
    EXPECT_STREQ("const", ns::to_string(ns::keyword::strict_const));
}

TEST(keyword, to_keyword_accept)
{
    {
        auto result = ns::to_keyword("var");

        ASSERT_TRUE(result.second);
        ASSERT_EQ(ns::keyword::strict_var, result.first);
    }

    {
        auto result = ns::to_keyword("const");

        ASSERT_TRUE(result.second);
        ASSERT_EQ(ns::keyword::strict_const, result.first);
    }
}

TEST(keyword, to_keyword_reject)
{
    ASSERT_FALSE(ns::to_keyword(nullptr).second);
    ASSERT_FALSE(ns::to_keyword(nullptr, 0U).second);
    ASSERT_FALSE(ns::to_keyword("").second);
    ASSERT_FALSE(ns::to_keyword("v").second);
    ASSERT_FALSE(ns::to_keyword("va").second);
    ASSERT_FALSE(ns::to_keyword("ver").second);
    ASSERT_FALSE(ns::to_keyword("varr").second);
}
