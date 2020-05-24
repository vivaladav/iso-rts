#pragma once

#include "Screen.h"

namespace game
{

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(Game * game);
    ~ScreenMainMenu();

    void Update() override;
    void Render() override;

    void OnApplicationQuit() override;
    void OnKeyUp(const lib::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

protected:
    void OnActive() override;
};

} // namespace game
