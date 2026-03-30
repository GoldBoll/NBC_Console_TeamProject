#include "InputManager.h"
#include <conio.h>

InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
{
    SetupDefaultBindings();
}

void InputManager::SetupDefaultBindings()
{
    keyMap['w'] = GameAction::MoveUp;
    keyMap['W'] = GameAction::MoveUp;
    keyMap['s'] = GameAction::MoveDown;
    keyMap['S'] = GameAction::MoveDown;
    keyMap['a'] = GameAction::MoveLeft;
    keyMap['A'] = GameAction::MoveLeft;
    keyMap['d'] = GameAction::MoveRight;
    keyMap['D'] = GameAction::MoveRight;

    keyMap['q'] = GameAction::Quit;
    keyMap['Q'] = GameAction::Quit;
    BindKey('i', GameAction::Help);
    BindKey('q', GameAction::Quit);

    // Battle Actions
    BindKey('1', GameAction::Action1);
    BindKey('2', GameAction::Action2);

    // Exploration Actions
    BindKey('`', GameAction::Dash);

    specialKeyMap[72] = GameAction::MoveUp;
    specialKeyMap[80] = GameAction::MoveDown;
    specialKeyMap[75] = GameAction::MoveLeft;
    specialKeyMap[77] = GameAction::MoveRight;
}

GameAction InputManager::PollInput() const
{
    if (!_kbhit()) return GameAction::None;

    int key = _getch();

    if (key == 0 || key == 224)
    {
        int scanCode = _getch();
        auto it = specialKeyMap.find(scanCode);
        if (it != specialKeyMap.end()) return it->second;
        return GameAction::None;
    }

    auto it = keyMap.find(key);
    if (it != keyMap.end()) return it->second;

    return GameAction::None;
}

void InputManager::BindKey(int keyCode, GameAction action)
{
    keyMap[keyCode] = action;
}
