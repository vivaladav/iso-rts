#pragma once

#include "Structure.h"

namespace game
{

class RadarTower : public Structure
{
public:
    RadarTower();

protected:
    void UpdateGraphics() override;

    void OnLinkedChanged() override;

private:
    void SetImage();
};

} // namespace game

