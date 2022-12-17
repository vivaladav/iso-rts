#include "PlayerAI.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "Player.h"
#include "GameObjects/Unit.h"

#include <sgl/utilities/LoadedDie.h>

#include <algorithm>
#include <iostream>

namespace game
{

PlayerAI::PlayerAI(Player * player)
    : mPlayer(player)
{

}

void PlayerAI::DecideActions()
{

}

ActionAI PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActions.empty())
        return { {0,0}, {0,0}, ACT_NOP, 0 };

    // return top action
    return PopAction();
}

void PlayerAI::PushAction(const ActionAI & action)
{
    mActions.emplace_back(action);
    std::push_heap(mActions.begin(), mActions.end(), ActionAiComp{});
}

ActionAI PlayerAI::PopAction()
{
    std::pop_heap(mActions.begin(), mActions.end(), ActionAiComp{});

    const ActionAI elem = mActions.back();
    mActions.pop_back();

    return elem;
}

void PlayerAI::AddNewAction(const ActionAI & action)
{
    // insert action if not already in the queue
    bool found = false;

    for(ActionAI & a : mActions)
    {
        // action already in queue -> update its priority
        if(a == action)
        {
            found = true;

            // update queue
            a.priority = action.priority;
            std::make_heap(mActions.begin(), mActions.end(), ActionAiComp{});

            break;
        }
    }

    // add new action
    if(!found)
    {
        std::cout << "NEW ACTION " << action.aid
                  << " - priority: " << action.priority << std::endl;
        PushAction(action);
    }
}

} // namespace game
