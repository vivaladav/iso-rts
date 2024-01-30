#pragma once

#include "GameButton.h"

namespace game
{

class ButtonPlanetMap : public GameButton
{
public:
    ButtonPlanetMap(sgl::sgui::Widget * parent);

    void SetTooltipText(const char * tooltip);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

class SecondaryButtonPlanetMap : public GameButton
{
public:
    SecondaryButtonPlanetMap(sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

class ButtonLeavePlanet : public GameButton
{
public:
    ButtonLeavePlanet(sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game
