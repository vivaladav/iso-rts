#pragma once

#include "Structure.h"

namespace game
{

class WallGate : public Structure
{
public:
    WallGate(GameObjectType subtype, int rows, int cols);

    bool IsOpen() const;
    bool Toggle();

    void SetGateType(GameObjectType type);

    static unsigned int GetCostEnergy(unsigned int level);
    static unsigned int GetCostMaterial(unsigned int level);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

private:
    GameObjectType mSubtype;

    bool mOpen = false;
};

inline bool WallGate::IsOpen() const
{
    return mOpen;
}

} // namespace game
