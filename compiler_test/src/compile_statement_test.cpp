#include "compiler_test_common.h"

// halberd::compiler
#include <halberd/compiler.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::compiler;

    auto compile_statement(const char* src)
    {
        return ns::parse(ns::rule::statement, src);
    }

    auto compile_statement(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::statement, std::move(tokens));
    }

    auto compile_statement_list(const char* src)
    {
        return ns::parse(ns::rule::statement_list, src);
    }

    auto compile_statement_list(std::vector<std::unique_ptr<halberd::lexer::token>> tokens)
    {
        return ns::parse(ns::rule::statement_list, std::move(tokens));
    }
}

TEST(compile_statement, expression_identifier_src)
{
    ASSERT_TRUE(compile_statement("my_var;"));
}

TEST(compile_statement, variable_declaration_i8)
{
    using namespace test;

    auto tokens = make_tokens(
        make_keyword(halberd::lexer::keyword::strict_var),
        make_keyword(halberd::lexer::keyword::strict_i8),
        make_identifier("my_var"),
        make_symbol(halberd::lexer::symbol::semicolon));

    ASSERT_TRUE(compile_statement(std::move(tokens)));
}

TEST(compile_statement, variable_declaration_src_i8)
{
    ASSERT_TRUE(compile_statement("var i8 my_var;"));
}

TEST(compile_statement, list_src)
{
    const char src[] = R"(
        var i32 v1;
        var i32 v2;
        var i32 v3;
        var i32 v4;

        v4 = (v1 + v2) / v3;
    )";

    ASSERT_TRUE(compile_statement_list(src));
}
