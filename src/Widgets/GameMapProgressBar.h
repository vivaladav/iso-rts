#pragma once

#include "Widgets/GameProgressBar.h"

namespace game
{

class GameMapProgressBar : public GameProgressBar
{
public:
    GameMapProgressBar(PlayerFaction faction, float time, sgl::sgui::Widget * parent = nullptr);

private:
    void OnUpdate(float delta) override;
};

} // namespace game
