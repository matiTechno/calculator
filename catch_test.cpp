#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "calculator.hpp"

TEST_CASE("simple add")
{
    REQUIRE(*calc_execute("5+9") == 14);
    REQUIRE(*calc_execute("1+66") == 67);
}

TEST_CASE("basic bad input handling")
{
    REQUIRE(!calc_execute("5.5+6"));
    REQUIRE(!calc_execute("goekgpwoe"));
}

TEST_CASE("simple subtraction")
{
    REQUIRE(*calc_execute("5 - 6") == -1);
}

TEST_CASE("single number")
{
    REQUIRE(*calc_execute("-5") == -5);
    REQUIRE(*calc_execute("+2") == 2);
    REQUIRE(*calc_execute("1") == 1);
    REQUIRE(*calc_execute("01") == 1);
    REQUIRE(*calc_execute("-(66)") == -66);
}
