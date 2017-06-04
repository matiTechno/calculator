#pragma once
#include <string>
#include <experimental/optional>

// works only with ints
// enter exit to exit
void calc_start();

namespace stde = std::experimental;

stde::optional<int> calc_execute(std::string string);
