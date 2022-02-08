#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class Game;

class PanelGameWon : public lib::sgui::Widget
{
public:
    PanelGameWon(Game * game, lib::sgui::Widget * parent = nullptr);
};

} // namespace game
