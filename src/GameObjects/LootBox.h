#pragma once

#include "Collectable.h"

namespace game
{

class LootBox : public Collectable
{
public:
    LootBox();

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();

private:

};

} // namespace game
