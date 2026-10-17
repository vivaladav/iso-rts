#pragma once

#include "Widgets/GameButton.h"

namespace game
{

// ---- BUTTON DISCORD ----
class ButtonExtDiscord : public GameButton
{
public:
    ButtonExtDiscord(sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

// ---- BUTTON FEEDBACK ----
class ButtonExtFeedback : public GameButton
{
public:
    ButtonExtFeedback(sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

// ---- BUTTON STEAM ----
class ButtonExtSteam : public GameButton
{
public:
    ButtonExtSteam(const std::string & url, sgl::sgui::Widget * parent);

private:
    void HandleMouseOver() override;
    void HandleButtonDown() override;
};

} // namespace game