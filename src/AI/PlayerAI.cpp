#include "PlayerAI.h"

#include "GameConstants.h"
#include "GameMap.h"
#include "Player.h"
#include "GameObjects/Structure.h"
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
    // STRUCTURES
    for(unsigned int i = 0; i < mPlayer->GetNumStructures(); ++i)
    {
        Structure * s = mPlayer->GetStructure(i);

        switch (s->GetObjectType())
        {
            case GameObjectType::OBJ_BASE:
                AddActionsBase(s);
            break;

            default:
            break;
        }
    }

    // UNITS
    for(unsigned int i = 0; i < mPlayer->GetNumUnits(); ++i)
    {
        // TODO
        //Unit * u = mPlayer->GetUnit(i);
    }
}

ActionAI PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActions.empty())
        return {};

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

void PlayerAI::AddActionsBase(Structure * s)
{
    const unsigned int numUnits = mPlayer->GetNumUnits();
    const unsigned int limitUnits = mPlayer->GetMaxUnits();

    // can't build more units
    if(numUnits >= limitUnits)
        return ;

    // create action
    ActionAINewUnit action;
    action.aid = AIA_NEW_UNIT;
    action.ObjSrc = s;
    action.priority = 0;

    // define base priority
    // MAX with 0 units and MIN with limit units
    const int maxPriority = 100;
    const int priority = maxPriority * (limitUnits - numUnits) / limitUnits;

    // DECIDE UNIT TYPE
    // TODO keep into consideration faction attitude
    // (i.e.: if faction is more inclined to attack then prefer soldier over builder)
    std::vector<int> priorities(NUM_UNIT_TYPES, priority);
    std::vector<int> costs(NUM_UNIT_TYPES, 0);

    // 1- exclude units that can't be built and check cost
    const int multEnergy = 1;
    const int multMaterial = 2;
    const int multBlobs = 10;
    const int multDiamonds = 20;
    int maxCost = 0;

    for(unsigned int i = 0; i < NUM_UNIT_TYPES; ++i)
    {
        const ObjectData & data = mPlayer->GetAvailableUnit(static_cast<UnitType>(i));

        if(data.objClass == OC_NULL || !mGm->CanCreateUnit(data, s, mPlayer))
        {
            priorities[i] = 0;
            continue;
        }

        costs[i] = data.costs[RES_ENERGY] * multEnergy + data.costs[RES_MATERIAL1] * multMaterial +
                   data.costs[RES_BLOBS] * multBlobs + data.costs[RES_DIAMONDS] * multDiamonds;

        if(costs[i] > maxCost)
            maxCost = costs[i];
    }

    // 2- apply bonuses
    const int bonusExistingUnit = -25;

    for(unsigned int i = 0; i < mPlayer->GetNumUnits(); ++i)
    {
        Unit * u = mPlayer->GetUnit(i);
        const unsigned int typeId = u->GetUnitType();

        if(0 == priorities[typeId])
            continue ;

        priorities[typeId] += bonusExistingUnit;

        // TODO consider cost
    }

    // 3- pick highest priority
    // for now picking first of list when priorities are the same
    for(unsigned int i = 0; i < NUM_UNIT_TYPES; ++i)
    {
        if(priorities[i] > action.priority)
        {
            action.priority = priorities[i];
            action.unitType = static_cast<UnitType>(i);
        }
    }

    // push action to the queue
    AddNewAction(action);
}

} // namespace game
