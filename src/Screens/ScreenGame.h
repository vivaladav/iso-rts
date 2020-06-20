#pragma once

#include "Screen.h"

namespace lib { namespace sgui { class Widget; } }

namespace game
{

class IsoMap;
class Player;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update() override;
    void Render() override;

    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

private:
    lib::sgui::Widget * CreatePlayerUI(Player * p);

    void SetHomeCell();

private:
    IsoMap * mMap = nullptr;
};

} // namespace game
