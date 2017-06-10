#pragma once
#include <string>
#include <optional>

// works only with ints

// enter exit to exit
void calc_start();

std::optional<int> calc_execute(std::string input);
