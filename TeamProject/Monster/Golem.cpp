#include "Golem.h"

Golem::Golem(bool _elite)
    : Monster("골렘 ",
             (_elite ? 60 : 50), // HP
             (_elite ? 13 : 9),  // ATK
             (_elite ? 14 : 10), // DEF
             (_elite ? 7  : 5),  // DEX
             (_elite ? 10 : 7),  // EXP
             _elite)
{
}
