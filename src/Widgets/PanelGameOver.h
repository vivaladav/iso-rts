#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class Game;

class PanelGameOver : public sgl::sgui::Widget
{
public:
    PanelGameOver(Game * game, sgl::sgui::Widget * parent = nullptr);
};

} // namespace game
