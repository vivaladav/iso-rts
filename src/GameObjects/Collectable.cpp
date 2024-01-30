#include "GameObjects/Collectable.h"

#include "GameObjects/CollectableGenerator.h"

namespace game
{

void Collectable::Collected()
{
    // reset generator timer to avoid to get new Collectable generated immediately
    if(mGen)
        mGen->ResetTimer();
}

} // namespace game
