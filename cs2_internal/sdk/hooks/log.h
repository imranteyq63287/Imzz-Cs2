#pragma once
#include <iostream>
#include <string>

namespace logger
{
    inline void log(const std::string& msg)
    {
        std::cout << "[LOG] " << msg << std::endl;
    }
}
