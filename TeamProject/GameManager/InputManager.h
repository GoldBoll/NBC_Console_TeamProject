#pragma once
#include <unordered_map>

enum class GameAction
{
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Help,
    Quit,
};

class InputManager
{
public:
    static InputManager& GetInstance();

    GameAction PollInput() const;
    void BindKey(int keyCode, GameAction action);

    InputManager(const InputManager&)            = delete;
    InputManager& operator=(const InputManager&) = delete;

private:
    InputManager();

    std::unordered_map<int, GameAction> keyMap;
    std::unordered_map<int, GameAction> specialKeyMap;

    void SetupDefaultBindings();
};
