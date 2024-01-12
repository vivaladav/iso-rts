#pragma once

#include "Structure.h"

namespace game
{

class Temple : public Structure
{
public:
    Temple();

private:
    void UpdateGraphics() override;

    void SetImage();
    void SetObjColors();
};

} // namespace game
