#include "Goblin.h"
#include <iostream>

Goblin::Goblin()
    : Monster("고블린", 4, 3, 3, 5)
{
    std::cout << "고블린 출현" << std::endl;
}
