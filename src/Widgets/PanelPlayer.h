#pragma once

#include <sgui/Widget.h>

namespace game
{

class Player;

class PanelPlayer : public lib::sgui::Widget
{
public:
    PanelPlayer(Player * p, lib::sgui::Widget * parent = nullptr);

};

} // namespace game
