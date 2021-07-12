#pragma once

#include "GameObject.h"

namespace game
{

class PracticeTarget : public GameObject
{
public:
    PracticeTarget(int rows, int cols);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();
};

} // namespace game
