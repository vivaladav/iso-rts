#include "Widgets/GameMapProgressBar.h"

namespace game
{

GameMapProgressBar::GameMapProgressBar(PlayerFaction faction, float time, sgl::sgui::Widget * parent)
    : GameProgressBar(faction, 0.f, time, parent)
{
}

void GameMapProgressBar::OnUpdate(float delta)
{
    IncValue(delta);
}

} // namespace game
