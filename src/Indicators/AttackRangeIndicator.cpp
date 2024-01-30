#include "Indicators/AttackRangeIndicator.h"

#include "GameData.h"
#include "Player.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

AttackRangeIndicator::AttackRangeIndicator()
    : IsoObject(1, 1)
{
}

void AttackRangeIndicator::SetFaction(PlayerFaction faction)
{
    using namespace sgl::graphic;

    // upate object body
    const unsigned int indSprite = IND_ATTACK_F1 + faction;

    auto tm = TextureManager::Instance();
    Texture * tex = tm->GetSprite(SpriteFileIndicators, indSprite);

    SetTexture(tex);
}

} // namespace game
