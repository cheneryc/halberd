#include "compiler_test_common.h"

#include <halberd/compiler.h>

#include <gtest/gtest.h>


namespace
{
    namespace ns = halberd::compiler;
}

TEST(compiler, variable_declaration_i8)
{
    using namespace test;

    auto tokens = make_tokens(
        make_keyword(halberd::lexer::keyword::strict_var),
        make_keyword(halberd::lexer::keyword::strict_i8),
        make_identifier("my_var"));

    ASSERT_TRUE(ns::compile(std::move(tokens)));
}

TEST(compiler, variable_declaration_src_i8)
{
    ASSERT_TRUE(ns::compile("var i8 my_var"));
}
