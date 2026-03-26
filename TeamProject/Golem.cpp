#include "Golem.h"
#include <iostream>

Golem::Golem()
    :Monster("골렘", 10, 12, 7, 7)
{
    std::cout << "골렘 출현" << std::endl;
}
