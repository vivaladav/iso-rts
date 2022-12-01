#pragma once

#include "GameButton.h"

namespace game
{

class ButtonPlanetMap : public GameButton
{
public:
    ButtonPlanetMap(sgl::sgui::Widget * parent);
};

class SecondaryButtonPlanetMap : public GameButton
{
public:
    SecondaryButtonPlanetMap(sgl::sgui::Widget * parent);
};

} // namespace game
