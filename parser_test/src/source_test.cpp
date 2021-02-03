#include <halberd/source.h>

#include <gtest/gtest.h>

#include <string>


namespace
{
    namespace ns = halberd::parser;

    auto create_source(std::string tokens)
    {
        return ns::make_source([str = tokens, i = 0U]() mutable
        {
            return (i < str.length()) ? str.at(i++) : '\0';
        });
    }
}

TEST(source, next_empty)
{
    auto src = ::create_source("");

    ASSERT_EQ(0U, src.get_position());
    const auto result = src.next();
    ASSERT_EQ(0U, src.get_position());

    ASSERT_FALSE(result.second);
}

TEST(source, next)
{
    auto fn = [](std::string tokens)
    {
        auto src_length = tokens.length();
        auto src = ::create_source(tokens);

        for (size_t i = 0; i < src_length; ++i)
        {
            ASSERT_EQ(i, src.get_position());
            const auto result = src.next();
            ASSERT_EQ(i + 1, src.get_position());

            ASSERT_TRUE(result.second);
            ASSERT_EQ(tokens.at(i), result.first);
        }

        ASSERT_EQ(src_length, src.get_position());
        const auto result = src.next();
        ASSERT_EQ(src_length, src.get_position());

        ASSERT_FALSE(result.second);
    };

    fn("n");
    fn("ne");
    fn("nex");
    fn("next");
}

TEST(source, try_advance_position_zero)
{
    auto  src = ::create_source("");
    auto& src_buffer = src.get_buffer();

    ASSERT_EQ(0U, src.get_position());
    ASSERT_EQ(0U, src_buffer.get_size());

    try_advance(src); // Test advancing with a position of zero succeeds and has no side effects

    ASSERT_EQ(0U, src.get_position());
    ASSERT_EQ(0U, src_buffer.get_size());
}

TEST(source, try_advance_buffer_size_one)
{
    auto fn = [](std::string tokens)
    {
        auto  src = ::create_source(std::move(tokens));
        auto& src_buffer = src.get_buffer();

        std::size_t i = 0U;

        while (src.next().second)
        {
            ++i;

            ASSERT_EQ(i, src.get_position());
            ASSERT_EQ(1U, src_buffer.get_size());

            try_advance(src);

            ASSERT_EQ(i, src.get_position());
            ASSERT_EQ(0U, src_buffer.get_size());
        }
    };

    fn("a");
    fn("ad");
    fn("adv");
    fn("adva");
    fn("advan");
    fn("advanc");
    fn("advance");
}

TEST(source, try_advance_buffer_size_many)
{
    auto fn = [](std::string tokens)
    {
        const auto length = tokens.length();

        auto  src = ::create_source(std::move(tokens));
        auto& src_buffer = src.get_buffer();

        while (src.next().second); // Consume all the tokens

        ASSERT_EQ(length, src.get_position());
        ASSERT_EQ(length, src_buffer.get_size());

        try_advance(src);

        ASSERT_EQ(length, src.get_position());
        ASSERT_EQ(0U, src_buffer.get_size());
    };

    fn("a");
    fn("ad");
    fn("adv");
    fn("adva");
    fn("advan");
    fn("advanc");
    fn("advance");
}

TEST(source, try_advance_not_advanceable)
{
    const std::string tokens = "advanceable";

    auto src = ::create_source(tokens);
    auto src_copy = decltype(src) { src, ns::source_flags::none }; // Create a copy of the source that cannot advance its buffer

    while (src_copy.next().second); // Consume all the tokens

    ASSERT_EQ(tokens.length(), src_copy.get_position());
    ASSERT_EQ(tokens.length(), src_copy.get_buffer().get_size());

    try_advance(src_copy);

    ASSERT_EQ(tokens.length(), src_copy.get_position());
    ASSERT_EQ(tokens.length(), src_copy.get_buffer().get_size());
}
