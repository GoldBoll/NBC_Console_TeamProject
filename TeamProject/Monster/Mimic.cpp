#include "Mimic.h"

Mimic::Mimic(bool _elite)
    : Monster("미믹 ",
             (_elite ? 30 : 20), // HP
             (_elite ? 10 : 7),  // ATK
             (_elite ? 10 : 7),  // DEF
             (_elite ? 7  : 5),  // DEX
             (_elite ? 10 : 7),  // EXP
             _elite)
{
}
