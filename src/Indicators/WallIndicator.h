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

enum GameObjectType : unsigned int;
enum PlayerFaction : unsigned int;

class WallIndicator : public IsoObject
{
public:
    WallIndicator();

    GameObjectType GetBlockType() const;

    void SetCost(float energy, float material);
    void ShowCost(bool val);

    void SetFaction(PlayerFaction faction);

    void SetBeforeAfterDirections(int br, int bc, int ar, int ac);

    void Render() override;

private:
    void CreateCostData();

    void UpdateImage();
    void UpdateCostColor();

private:
    enum WallBlock : unsigned int
    {
        WB_HORIZONTAL,
        WB_VERTICAL,
        WB_TOP_LEFT,
        WB_TOP_RIGHT,
        WB_BOTTOM_LEFT,
        WB_BOTTOM_RIGHT,

        NUM_WALL_BLOCKS,

        WB_INVALID
    };

    lib::graphic::Image * mIconEnergy = nullptr;
    lib::graphic::Text * mTxtCostEnergy = nullptr;
    lib::graphic::Image * mIconMaterial = nullptr;
    lib::graphic::Text * mTxtCostMaterial = nullptr;
    unsigned int mColorCost = 0xFFFFFFFF;

    PlayerFaction mFaction;
    float mCostEnergy = 0.f;
    float mCostMaterial = 0.f;

    WallBlock mBlock = WB_INVALID;
};

} // namespace game
