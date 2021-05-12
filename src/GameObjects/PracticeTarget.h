#pragma once

#include "GameObject.h"

namespace game
{

class PracticeTarget : public GameObject
{
public:
    PracticeTarget(int rows, int cols);

protected:
    void UpdateImage() override;

private:
    void SetImage();
};

} // namespace game
