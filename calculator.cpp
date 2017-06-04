#include "calculator.hpp"
#include <iostream>
#include <algorithm>

void calc_start()
{
    std::string input;
    for(;;)
    {
        std::getline(std::cin, input);
        if(input == "exit")
            return;

        if(auto ret = calc_execute(input))
            std::cout << ret.value() << std::endl;
        else
            std::cout << "error: bad input data" << std::endl;
    }
}

stde::optional<int> calc_execute(std::string string)
{
    string.erase(std::remove(string.begin(), string.end(), ' '), string.end());

    for(auto c: string)
    {
        if(c != '(' && c != ')' && c != '-' && c != '+' && c != '*' && c != '/' && (c < 48 || c > 57))
            return {};
    }

    return {};
}
