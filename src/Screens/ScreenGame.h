#pragma once

#include "Screen.h"

namespace game
{

class IsoMap;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update() override;
    void Render() override;

    void OnApplicationQuit() override;
    void OnKeyUp(const lib::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

private:
    void OnActive() override;

private:
    IsoMap * mMap = nullptr;
};

} // namespace game
