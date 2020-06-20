#pragma once

#include <sgui/Widget.h>

namespace game
{

class Player;

class PanelPlayer : public lib::sgui::Widget
{
public:
    PanelPlayer(Player * p, lib::sgui::Widget * parent = nullptr);

private:
    void CreatePanelCell();

    void CreatePanelUnits();

private:
    lib::sgui::Widget * mPanelCell = nullptr;

    lib::sgui::Widget * mPanelUnits = nullptr;

};

} // namespace game
