#pragma once

#include "IsoObject.h"

namespace lib { namespace graphic { class Renderable; } }

namespace game
{

enum PlayerFaction : unsigned int;

class ConquestIndicator : public IsoObject
{
public:
    ConquestIndicator();

    void SetFaction(PlayerFaction faction);

    void SetCost(float val);
    void ShowCost(bool val);

    void Render() override;

private:
    void CreateLabelCost();

    void OnPositionChanged() override;

private:
    lib::graphic::Renderable * mTxtDummy = nullptr;
    lib::graphic::Renderable * mTxtCost = nullptr;
    lib::graphic::Renderable * mCurrTxt = nullptr;

    float mCost = -1.f;
};

} // namespace game
