#include "PlayerAI.h"

#include "GameMap.h"

namespace game
{

PlayerAI::PlayerAI(Player * player)
    : mPlayer(player)
{

}

void PlayerAI::DecideActions(GameMap * gm)
{

}

ActionAI PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActions.empty())
        return { ACT_NOP, 0, {0,0}, {0,0} };

    // return top action after removing it from the queue
    ActionAI action = mActions.top();
    mActions.pop();

    return action;
}

} // namespace game
