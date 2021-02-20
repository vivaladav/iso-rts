#include "Indicators/MoveIndicator.h"

#include <graphic/TextureManager.h>

namespace game
{

MoveIndicator::MoveIndicator()
    : IsoObject(1, 1)
{
    using namespace lib::graphic;

    Texture * tex = TextureManager::Instance()->GetTexture("data/img/selection-f1.png");
    SetTexture(tex);
}

void MoveIndicator::Render()
{
    IsoObject::Render();
}

} // namespace game
