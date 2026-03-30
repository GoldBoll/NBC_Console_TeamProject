#include "Orc.h"

Orc::Orc(bool _elite)
    : Monster("오크 ",
             (_elite ? 22 : 15), // HP
             (_elite ? 8  : 5),  // ATK
             (_elite ? 6  : 4),  // DEF
             (_elite ? 4  : 3),  // DEX
             (_elite ? 8  : 5),  // EXP
             _elite)
{
}
