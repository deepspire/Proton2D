#include <iostream>
#include "logman.hh"

void ProtonLog(const char *msg)
{
    // TODO: добавить поддержку форматирования (%s, ...)
    std::cout << "[BURGER2D] " << msg << std::endl;
}