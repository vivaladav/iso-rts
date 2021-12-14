#include "States/StateFactionSelection.h"

#include "Screens/ScreenFactionSelection.h"
#include "States/StatesIds.h"

namespace game
{

StateFactionSelection::StateFactionSelection(Game * game)
    : BaseGameState(StateId::FACTION_SEL, game)
{
}

void StateFactionSelection::OnActive()
{
    mScreen = new ScreenFactionSelection(mGame);
}

void StateFactionSelection::OnInactive()
{
    delete mScreen;
    mScreen = nullptr;
}

} // namespace game
