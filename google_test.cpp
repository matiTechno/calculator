#include <gtest/gtest.h>
#include "calc.hpp"

using namespace calc;

TEST(CalcTest, Multiplying)
{
    EXPECT_EQ(55, *execute("5  *11"));
}

TEST(CalcTest, DividingBy0)
{
    ASSERT_FALSE(execute("55 / 0"));
}

TEST(CalcTest, FloatNumberHandling)
{
    ASSERT_FALSE(execute("0.5f"));
}

TEST(CalcTest, SingleNumber)
{
    EXPECT_EQ(-5, *execute("-5"));
}

TEST(CalcTest, SingleNumberAndParentheses)
{
    EXPECT_EQ(11, *execute("(11)"));
}

TEST(CalcTest, MoreComplicatedExpression)
{
    EXPECT_EQ(-31, *execute("(1 + 4 + 1 + 5 * (1 + 4)) * (-1)*(2)/2"));
}

TEST(CalcTest, ParenthesesMismatch)
{
    ASSERT_FALSE(execute("(-1"));
    ASSERT_FALSE(execute("-1)"));
}

TEST(CalcTest, WhitespaceHandling)
{
    EXPECT_EQ(-4, *execute("1   +(-5)"));
}

TEST(CalcTest, SingleNumberMinusBeforePrentheses)
{
    EXPECT_EQ(666, *execute("-(-666)"));
}

TEST(CalcTest, OperatorMismatch)
{
    ASSERT_FALSE(execute("1 ++ 2"));
    ASSERT_FALSE(execute(" 1 + -(55)"));
}

TEST(CalcTest, EmptyParentheses)
{
    ASSERT_FALSE(execute("-6()"));
    ASSERT_FALSE(execute("()"));
}
