#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class Game;

class PanelGameWon : public sgl::sgui::Widget
{
public:
    PanelGameWon(Game * game, sgl::sgui::Widget * parent = nullptr);
};

} // namespace game
