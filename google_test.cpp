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
