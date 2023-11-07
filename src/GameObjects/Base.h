#pragma once

#include "Structure.h"

namespace game
{

class Base : public Structure
{
public:
    Base();

protected:
    void UpdateGraphics() override;

private:
    void SetImage();
};

} // namespace game
