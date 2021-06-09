#include <halberd/source_buffer.h>

#include <gtest/gtest.h>

#include <memory> // std::unique_ptr, std::make_unique

#include <cstddef> // std::size_t


namespace
{
    namespace ns = halberd::parser;

    auto create_source_buffer(int tokens)
    {
        return ns::make_source_buffer([i = tokens]() mutable { return i--; });
    }

    auto create_source_buffer_noncopyable(int tokens)
    {
        return ns::make_source_buffer([i = tokens]() mutable
        {
            std::unique_ptr<int> token;

            if (i > 0)
            {
                token = std::make_unique<int>(i--);
            }

            return token;
        },
            [](std::unique_ptr<int>& int_ptr)
        {
            return *int_ptr;
        });
    }
}

TEST(source_buffer, zero_tokens)
{
    auto sb = ::create_source_buffer(0);

    ASSERT_EQ(0U, sb->get_size());
    const auto result = sb->at(0U);
    ASSERT_EQ(0U, sb->get_size());

    ASSERT_FALSE(result.second);
}

TEST(source_buffer, zero_tokens_noncopyable)
{
    auto sb = ::create_source_buffer_noncopyable(0);

    ASSERT_EQ(0U, sb->get_size());
    const auto result = sb->at(0U);
    ASSERT_EQ(0U, sb->get_size());

    ASSERT_FALSE(result.second);
}

TEST(source_buffer, one_token)
{
    auto sb = ::create_source_buffer(1);

    {
        ASSERT_EQ(0U, sb->get_size());
        const auto result = sb->at(0U);
        ASSERT_EQ(1U, sb->get_size());

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first);
    }

    {
        ASSERT_EQ(1U, sb->get_size());
        const auto result = sb->at(0U); // Check that the result of the first call at offset 0 has been stored in the buffer
        ASSERT_EQ(1U, sb->get_size());

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first);
    }

    {
        ASSERT_EQ(1U, sb->get_size());
        const auto result = sb->at(1U);
        ASSERT_EQ(1U, sb->get_size());

        ASSERT_FALSE(result.second);
    }
}

TEST(source_buffer, one_token_noncopyable)
{
    auto sb = ::create_source_buffer_noncopyable(1);

    {
        ASSERT_EQ(0U, sb->get_size());
        const auto result = sb->at(0U);
        ASSERT_EQ(1U, sb->get_size());

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first);
    }

    {
        ASSERT_EQ(1U, sb->get_size());
        const auto result = sb->at(0U); // Check that the result of the first call at offset 0 has been stored in the buffer
        ASSERT_EQ(1U, sb->get_size());

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first);
    }

    {
        ASSERT_EQ(1U, sb->get_size());
        const auto result = sb->at(1U);
        ASSERT_EQ(1U, sb->get_size());

        ASSERT_FALSE(result.second);
    }
}

TEST(source_buffer, many_tokens)
{
    constexpr std::size_t tokens = 32U;
    auto sb = ::create_source_buffer(tokens);

    for (std::size_t i = 0U; i < tokens; ++i)
    {
        ASSERT_EQ(i, sb->get_size());
        const auto result = sb->at(i);
        ASSERT_EQ(i + 1U, sb->get_size());

        ASSERT_TRUE(result.second);
        ASSERT_EQ(tokens - i, result.first);
    }

    ASSERT_EQ(tokens, sb->get_size());
    const auto result = sb->at(tokens);
    ASSERT_EQ(tokens, sb->get_size());

    ASSERT_FALSE(result.second);
}

TEST(source_buffer, many_tokens_noncopyable)
{
    constexpr std::size_t tokens = 32U;
    auto sb = ::create_source_buffer_noncopyable(tokens);

    for (std::size_t i = 0U; i < tokens; ++i)
    {
        ASSERT_EQ(i, sb->get_size());
        const auto result = sb->at(i);
        ASSERT_EQ(i + 1U, sb->get_size());

        ASSERT_TRUE(result.second);
        ASSERT_EQ(tokens - i, result.first);
    }

    ASSERT_EQ(tokens, sb->get_size());
    const auto result = sb->at(tokens);
    ASSERT_EQ(tokens, sb->get_size());

    ASSERT_FALSE(result.second);
}

TEST(source_buffer, advance_distance_zero)
{
    auto fn = [](int tokens)
    {
        auto sb = ::create_source_buffer(tokens);

        ASSERT_EQ(0U, sb->get_size());
        sb->advance(0U); // Check that advancing a zero distance has no effect
        ASSERT_EQ(0U, sb->get_size());

        const auto result = sb->at(tokens - 1U); // Generate the requested number of tokens

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first); // The nth token should have a value of one

        ASSERT_EQ(tokens, sb->get_size());
        sb->advance(0U); // Check that advancing a zero distance has no effect
        ASSERT_EQ(tokens, sb->get_size());
    };

    fn(1);
    fn(2);
    fn(4);
    fn(8);
}

TEST(source_buffer, advance_distance_less)
{
    auto fn = [](int tokens)
    {
        auto sb = ::create_source_buffer(tokens);
        const auto result = sb->at(tokens - 1U); // Generate the requested number of tokens

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first); // The nth token should have a value of one

        ASSERT_EQ(tokens, sb->get_size());
        sb->advance(tokens - 1U); // Check that advancing one less than the number of tokens leaves a single element in the buffer
        ASSERT_EQ(1U, sb->get_size());
    };

    fn(1);
    fn(2);
    fn(4);
    fn(8);
}

TEST(source_buffer, advance_distance_equal)
{
    auto fn = [](int tokens)
    {
        auto sb = ::create_source_buffer(tokens);
        const auto result = sb->at(tokens - 1U); // Generate the requested number of tokens

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first); // The nth token should have a value of one

        ASSERT_EQ(tokens, sb->get_size());
        sb->advance(tokens); // Check that advancing the number of tokens leaves zero elements in the buffer
        ASSERT_EQ(0U, sb->get_size());
    };

    fn(1);
    fn(2);
    fn(4);
    fn(8);
}

TEST(source_buffer, advance_distance_greater)
{
    auto fn = [](int tokens)
    {
        auto sb = ::create_source_buffer(tokens);
        const auto result = sb->at(tokens - 1U); // Generate the requested number of tokens

        ASSERT_TRUE(result.second);
        ASSERT_EQ(1, result.first); // The nth token should have a value of one

        ASSERT_EQ(tokens, sb->get_size());
        ASSERT_THROW(sb->advance(tokens + 1U), std::exception); // Check that advancing one greater than the number of tokens throw an exception
    };

    fn(1);
    fn(2);
    fn(4);
    fn(8);
}

TEST(source_buffer, exhausted)
{
    std::size_t count = 0;

    auto sb = ns::make_source_buffer([&count]() mutable
    {
        ++count; return 0U;
    });

    const auto res1 = sb->at(0U); // Attempt to generate a single token (the _is_exhausted flag should be set)
    ASSERT_FALSE(res1.second);

    const auto res2 = sb->at(0U); // Attempt to generate a single token (does nothing as the _is_exhausted flag is set)
    ASSERT_FALSE(res2.second);

    ASSERT_EQ(1U, count); // Ensure the token factory function was only called once
}
