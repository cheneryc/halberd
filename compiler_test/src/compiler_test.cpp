#include <halberd/compiler.h>

#include <gtest/gtest.h>

#include <iterator> // std::begin, std::end, std::make_move_iterator
#include <string> // std::string
#include <utility> // std::forward


namespace
{
    namespace ns = halberd::compiler;

    std::unique_ptr<halberd::lexer::token_identifier_reserved> make_keyword(halberd::lexer::keyword keyword)
    {
        return std::make_unique<halberd::lexer::token_identifier_reserved>(keyword);
    }

    std::unique_ptr<halberd::lexer::token_identifier> make_identifier(std::string identifier)
    {
        return std::make_unique<halberd::lexer::token_identifier>(std::move(identifier));
    }

    template<typename... Ts>
    std::vector<std::unique_ptr<halberd::lexer::token>> make_tokens(Ts&&... t)
    {
        // Cannot use move semantics with std::initializer_list so use an array as an intermediary instead
        std::unique_ptr<halberd::lexer::token> tokens_init[] = {
            std::forward<Ts>(t)...
        };

        std::vector<std::unique_ptr<halberd::lexer::token>> tokens = {
            std::make_move_iterator(std::begin(tokens_init)),
            std::make_move_iterator(std::end(tokens_init)),
        };

        return tokens;
    }
}

TEST(compiler, variable_declaration_i8)
{
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
