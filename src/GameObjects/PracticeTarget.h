#pragma once

#include "Structure.h"

namespace game
{

class PracticeTarget : public Structure
{
public:
    PracticeTarget();

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();
};

} // namespace game
