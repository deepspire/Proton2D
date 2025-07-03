#pragma once
#include <iostream>
#include <sstream>

namespace Proton
{
    template<typename... logArgs>
    void Log(logArgs&&... args) {
        std::stringstream ss;
        (ss << ... << args);
        std::cout << "[PROTON2D] " << ss.str() << std::endl;
    }   
}