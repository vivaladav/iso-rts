#pragma once

#include "IsoObject.h"

namespace lib { namespace graphic { class Text; } }

namespace game
{

enum PlayerFaction : unsigned int;

class WallIndicator : public IsoObject
{
public:
    WallIndicator();

    void SetCost(float energy, float material);
    void ShowCost(bool val);

    void SetFaction(PlayerFaction faction);

    void Render() override;

private:
    void CreateCostData();

    void UpdateImage();

private:
    lib::graphic::Text * mTxtCostEnergy = nullptr;
    lib::graphic::Text * mTxtCostMAterial = nullptr;
    unsigned int mColorCost = 0;

    PlayerFaction mFaction;
    float mCostEnergy = 0.f;
    float mCostMaterial = 0.f;
};

} // namespace game
