#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl  { namespace graphic  { class Renderable; } }

namespace game
{

enum PlayerFaction : unsigned int;

class CountdownLabel : public sgl::sgui::Widget
{
public:
    CountdownLabel(PlayerFaction faction, int seconds, sgl::sgui::Widget * parent = nullptr);

private:
    void UpdateDigits();

    void HandlePositionChanged() override;

    void SetPositions();

    void OnUpdate(float delta) override;

private:
    float mTime = 0;
    int mTimeInt = 0;

    PlayerFaction mFaction;

    sgl::graphic::Renderable * mTxt = nullptr;
};

} // namespace game
