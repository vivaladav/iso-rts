#pragma once

#include "Structure.h"

namespace game
{

class RadarStation : public Structure
{
public:
    RadarStation(int rows, int cols);

protected:
    void UpdateGraphics() override;

    void OnLinkedChanged() override;

private:
    void SetImage();
};

} // namespace game

