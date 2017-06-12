#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "calc.hpp"

using namespace calc;

TEST_CASE("simple add")
{
    REQUIRE(*execute("5+9") == 14);
    REQUIRE(*execute("1+66") == 67);
}

TEST_CASE("bad input handling")
{
    REQUIRE(!execute("5.5+6"));
    REQUIRE(!execute("goekgpwoe"));
}

TEST_CASE("simple subtraction")
{
    REQUIRE(*execute("5 - 6") == -1);
}

TEST_CASE("single number")
{
    REQUIRE(*execute("-5") == -5);
    REQUIRE(*execute("+2") == 2);
    REQUIRE(*execute("1") == 1);
    REQUIRE(*execute("01") == 1);
    REQUIRE(*execute("-(66)") == -66);
}
