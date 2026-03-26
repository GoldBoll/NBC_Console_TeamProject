#include "Warrior.h"
#include <iostream>
using namespace std;

Warrior::Warrior() : Player()
{
    job = "전사";
    cout << "* 전사 입니다." << endl;
}
//void Warrior::attack()
// {
//    //cout << "* 검 휘두르기" << endl;
//}
//
//void Warrior::attack(Monster* monster)
// {
//
//}

void Warrior::TryMove(int _dx, int _dy, Map& _map)
{

}
