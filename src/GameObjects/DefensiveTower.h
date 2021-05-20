#pragma once

#include "GameObject.h"

namespace game
{

class DefensiveTower : public GameObject
{
public:
    DefensiveTower(int rows, int cols);

protected:
    void UpdateImage() override;

private:
    void SetImage();
};

} // namespace game
