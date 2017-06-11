#include <gtest/gtest.h>
#include "calculator.hpp"

TEST(CalcTest, Multiplying)
{
    EXPECT_EQ(55, *calc_execute("5  *11"));
}

TEST(CalcTest, DividingBy0)
{
    ASSERT_FALSE(calc_execute("55 / 0"));
}

TEST(CalcTest, FloatNumberHandling)
{
    ASSERT_FALSE(calc_execute("0.5f"));
}

TEST(CalcTest, SingleNumber)
{
    EXPECT_EQ(-5, *calc_execute("-5"));
}

TEST(CalcTest, SingleNumberAndParentheses)
{
    EXPECT_EQ(11, *calc_execute("(11)"));
}

TEST(CalcTest, MoreCompicatedExpression)
{
    EXPECT_EQ(-31, *calc_execute("(1 + 4 + 1 + 5 * (1 + 4)) * (-1)*(2)/2"));
}

TEST(CalcTest, ParenthesesMismatch)
{
    ASSERT_FALSE(calc_execute("(-1"));
    ASSERT_FALSE(calc_execute("-1)"));
}

TEST(CalcTest, WhitespaceHandling)
{
    EXPECT_EQ(-4, *calc_execute("1   +(-5)"));
}

TEST(CalcTest, OneNumberMinusBeforePrentheses)
{
    EXPECT_EQ(666, *calc_execute("-(-666)"));
}

TEST(CalcTest, OperatorMismatch)
{
    ASSERT_FALSE(calc_execute("1 ++ 2"));
    ASSERT_FALSE(calc_execute(" 1 + -(55)"));
}
