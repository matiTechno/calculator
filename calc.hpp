#pragma once
#include <string>
#include <optional>

namespace calc
{

// works only with ints
// enter exit to exit
void start();

std::optional<int> execute(const std::string& input);

}
