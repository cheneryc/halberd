#include <halberd/lexer.h>

#include <gtest/gtest.h>


using namespace halberd;

TEST(lexer, lexer_status_true)
{
    const bool status = lexer::get_status();

    ASSERT_EQ(true, status);
}

/*TEST(lexer, lexer_status_false)
{
    const bool status = lexer::get_status();

    ASSERT_EQ(false, status);
}*/
