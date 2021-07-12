#pragma once

#include "GameObject.h"

namespace game
{

class Base : public GameObject
{
public:
    Base(int rows, int cols);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();
};

} // namespace game
