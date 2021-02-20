#pragma once

#include "IsoObject.h"

namespace game
{

class MoveIndicator : public IsoObject
{
public:
    MoveIndicator();

    void Render() override;
};

} // namespace game
