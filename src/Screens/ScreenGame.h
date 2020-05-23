#pragma once

#include "Screen.h"

namespace game
{

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update() override;

    void OnApplicationQuit() override;
    void OnKeyUp(const lib::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

protected:
    void OnActive() override;
};

} // namespace game
