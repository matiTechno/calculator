#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "calculator.hpp"

TEST_CASE("simple add")
{
    auto ret = calc_execute("5+9");
    REQUIRE(ret);
    REQUIRE(*calc_execute("5+9") == 14);
}

TEST_CASE("bad input handling")
{
    REQUIRE(calc_execute("5.5+6") != 0);
    REQUIRE(calc_execute("goekgpwoe") != 0);
}
