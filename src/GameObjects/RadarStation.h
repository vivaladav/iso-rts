#pragma once

#include "GameObject.h"

namespace game
{

class RadarStation : public GameObject
{
public:
    RadarStation(int rows, int cols);

protected:
    void UpdateImage() override;

    void OnLinkedChanged() override;

private:
    void SetImage();
};

} // namespace game

