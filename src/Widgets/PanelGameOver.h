#pragma once

#include <sgui/Widget.h>

namespace game
{

class Game;

class PanelGameOver : public lib::sgui::Widget
{
public:
    PanelGameOver(Game * game, lib::sgui::Widget * parent = nullptr);
};

} // namespace game
