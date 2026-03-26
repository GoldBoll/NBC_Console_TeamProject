#include "Orc.h"
#include <iostream>

Orc::Orc()
    : Monster("오크", 6, 6, 3, 5)
{
    std::cout << "오크 출현" << std::endl;
}
