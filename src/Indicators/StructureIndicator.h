#pragma once

#include "IsoObject.h"

namespace lib
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

struct ObjectData;

enum PlayerFaction : unsigned int;

class StructureIndicator : public IsoObject
{
public:
    StructureIndicator(const ObjectData & data);

    void SetCost(float energy, float material);
    void ShowCost(bool val);

    void SetFaction(PlayerFaction faction);

    void Render() override;

private:
    void CreateCostData();

    void UpdateCostColor();

private:
    lib::graphic::Image * mIconEnergy = nullptr;
    lib::graphic::Text * mTxtCostEnergy = nullptr;
    lib::graphic::Image * mIconMaterial = nullptr;
    lib::graphic::Text * mTxtCostMaterial = nullptr;
    unsigned int mColorCost = 0xFFFFFFFF;

    PlayerFaction mFaction;
    float mCostEnergy = 0.f;
    float mCostMaterial = 0.f;
};

} // namespace game
