#include <halberd/compiler.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::compiler;
}

TEST(compiler, validate_identifiers_unknown)
{
    const char src[] = R"(
        var i32 v1;
        var i32 v2;

        v3 = v1 + v2;
    )";

    // 'v3' is an unknown identifier
    ASSERT_THROW(ns::compile(src), std::exception);
}

TEST(compiler, validate_identifiers_duplicate)
{
    const char src[] = R"(
        var i32 v1;
        var i32 v1;
    )";

    // 'v1' is an duplicate variable identifier
    ASSERT_THROW(ns::compile(src), std::exception);
}

TEST(compiler, validate_types_mismatch_operator_assignment)
{
    const char src[] = R"(
        var i16 v1;
        var i32 v2;

        v1 = v2;
    )";

    // 'v1' and 'v2' types do not match
    ASSERT_THROW(ns::compile(src), std::exception);
}

TEST(compiler, validate_types_mismatch_operator_binary)
{
    const char src[] = R"(
        var i16 v1;
        var i32 v2;

        v1 + v2;
    )";

    // 'v1' and 'v2' types do not match
    ASSERT_THROW(ns::compile(src), std::exception);
}
