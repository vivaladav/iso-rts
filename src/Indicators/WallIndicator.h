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

enum PlayerFaction : unsigned int;

class WallIndicator : public IsoObject
{
public:
    WallIndicator();

    void SetCost(float energy, float material);
    void ShowCost(bool val);

    void SetFaction(PlayerFaction faction);

    void SetBeforeAfterDirections(int br, int bc, int ar, int ac);

    void Render() override;

private:
    void CreateCostData();

    void UpdateImage();

private:
    enum Block : unsigned int
    {
        HORIZONTAL,
        VERTICAL,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,

        NUM_BLOCKS,

        INVALID
    };

    lib::graphic::Image * mIconEnergy = nullptr;
    lib::graphic::Text * mTxtCostEnergy = nullptr;
    lib::graphic::Image * mIconMaterial = nullptr;
    lib::graphic::Text * mTxtCostMaterial = nullptr;
    unsigned int mColorCost = 0xd6f5dbff;

    PlayerFaction mFaction;
    float mCostEnergy = 0.f;
    float mCostMaterial = 0.f;

    Block mBlock = INVALID;
};

} // namespace game
