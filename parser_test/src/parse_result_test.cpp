#include <halberd/parse_result.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::parser;

    struct test_struct
    {
        test_struct(char ch1, char ch2) : ch1(ch1), ch2(ch2)
        {
        }

        ~test_struct() noexcept // User-defined destructor makes type non-trivial and therefore requires util::optional::~optional to explicitly call the destructor
        {
        }

        char ch1;
        char ch2;
    };
}

TEST(parse_result, zero_values_failure)
{
    ns::parse_result<> res = { false };
    ASSERT_FALSE(res);
}

TEST(parse_result, zero_values_success)
{
    ns::parse_result<> res = { true };
    ASSERT_TRUE(res);
}

TEST(parse_result, one_value_failure)
{
    ns::parse_result<int> res = {};
    ASSERT_FALSE(res);
}

TEST(parse_result, one_value_failure_get)
{
    ns::parse_result<int> res = {};

    ASSERT_FALSE(res);
    ASSERT_THROW(res.get(), std::exception);
}

TEST(parse_result, one_value_success)
{
    ns::parse_result<int> res = 1;
    ASSERT_TRUE(res);
}

TEST(parse_result, one_value_success_get)
{
    ns::parse_result<int> res = 1;

    ASSERT_TRUE(res);
    ASSERT_EQ(1, res.get());
}

TEST(parse_result, multiple_values_failure)
{
    ns::parse_result<int, test_struct> res = {};
    ASSERT_FALSE(res);
}

TEST(parse_result, multiple_values_failure_get)
{
    ns::parse_result<int, test_struct> res = {};

    ASSERT_FALSE(res);
    ASSERT_THROW(res.get<0U>(), std::exception);
    ASSERT_THROW(res.get<1U>(), std::exception);

    const ns::parse_result<int, test_struct> res_const = {};

    ASSERT_FALSE(res_const);
    ASSERT_THROW(res_const.get<0U>(), std::exception);
    ASSERT_THROW(res_const.get<1U>(), std::exception);
}

TEST(parse_result, multiple_values_success)
{
    ns::parse_result<int, test_struct> res = { 1, test_struct{'a', 'b'} };
    ASSERT_TRUE(res);
}

TEST(parse_result, multiple_values_success_get)
{
    ns::parse_result<int, test_struct> res = { 1, test_struct{'a', 'b'} };

    ASSERT_TRUE(res);
    ASSERT_EQ(1, res.get<0U>());
    ASSERT_EQ('a', res.get<1U>().ch1);
    ASSERT_EQ('b', res.get<1U>().ch2);

    const ns::parse_result<int, test_struct> res_const = { 1, test_struct{'a', 'b'} };

    ASSERT_TRUE(res_const);
    ASSERT_EQ(1, res_const.get<0U>());
    ASSERT_EQ('a', res_const.get<1U>().ch1);
    ASSERT_EQ('b', res_const.get<1U>().ch2);
}
