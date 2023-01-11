#pragma once

#include "Structure.h"

namespace game
{

class RadarTower : public Structure
{
public:
    RadarTower(int rows, int cols);

protected:
    void UpdateGraphics() override;

    void OnLinkedChanged() override;

private:
    void SetImage();
};

} // namespace game

