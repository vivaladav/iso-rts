#pragma once

#include "IsoObject.h"
#include "GameObjects/GameObjectTypes.h"

namespace sgl
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

    game::GameObjectVariantId GetBlockType() const;

    void SetCost(unsigned int energy, unsigned int material);
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

    sgl::graphic::Image * mIconEnergy = nullptr;
    sgl::graphic::Text * mTxtCostEnergy = nullptr;
    sgl::graphic::Image * mIconMaterial = nullptr;
    sgl::graphic::Text * mTxtCostMaterial = nullptr;
    unsigned int mColorCost = 0xFFFFFFFF;

    PlayerFaction mFaction;
    unsigned int mCostEnergy = 0;
    unsigned int mCostMaterial = 0;

    WallBlock mBlock = WB_INVALID;
};

} // namespace game
