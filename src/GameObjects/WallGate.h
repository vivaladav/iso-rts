#pragma once

#include "Structure.h"

namespace game
{

class WallGate : public Structure
{
public:
    enum GateOrientation : GameObjectVariantId
    {
        HORIZ,
        VERT,

        NUM_GATE_ORIENTATIONS
    };

public:
    WallGate(GameObjectVariantId orientation);

    bool IsOpen() const;
    bool Toggle();

    static unsigned int GetCostEnergy(unsigned int level);
    static unsigned int GetCostMaterial(unsigned int level);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

private:
    bool mOpen = false;
};

inline bool WallGate::IsOpen() const
{
    return mOpen;
}

} // namespace game
