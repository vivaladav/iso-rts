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

PlayerAI::~PlayerAI()
{
    ClearActionsDone();
}

void PlayerAI::Update(float delta)
{
    // TODO track time and keep it into consideration when defining priorities
    // TODO use memory pools for actions
    ClearActionsDone();

    DecideActions();
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
        Unit * u = mPlayer->GetUnit(i);
        AddActionsUnit(u);
    }
}

const ActionAI * PlayerAI::GetNextAction()
{
    // return NOP action if queue is empty
    if(mActionsTodo.empty())
        return nullptr;

    // return top action
    return PopAction();
}

void PlayerAI::ClearActionsDone()
{
    for(ActionAI * a : mActionsDone)
        delete a;

    mActionsDone.clear();
}

void PlayerAI::PushAction(ActionAI * action)
{
    mActionsTodo.emplace_back(action);
    std::push_heap(mActionsTodo.begin(), mActionsTodo.end(), ActionAiComp{});
}

const ActionAI * PlayerAI::PopAction()
{
    std::pop_heap(mActionsTodo.begin(), mActionsTodo.end(), ActionAiComp{});

    ActionAI * elem = mActionsTodo.back();
    mActionsTodo.pop_back();

    mActionsDone.push_back(elem);

    return elem;
}

void PlayerAI::AddNewAction(ActionAI * action)
{
    // insert action if not already in the queue
    bool found = false;

    for(ActionAI * a : mActionsTodo)
    {
        // same action type
        if(a->aid == action->aid)
        {
            if(AIA_NEW_UNIT == action->aid)
            {
                auto au1 = static_cast<ActionAINewUnit *>(a);
                auto au2 = static_cast<ActionAINewUnit *>(action);

                found = au1->unitType == au2->unitType;
            }
            // compare basic actions
            else
            {
                found = a->ObjSrc == action->ObjSrc && a->ObjDst == action->ObjDst &&
                        a->cellSrc == action->cellSrc && a->cellDst == action->cellDst;
            }
        }

        // action already in queue -> update its priority
        if(found)
        {
            std::cout << "PlayerAI::AddNewAction - UPDATING ACTION " << action->aid << " - old priority: "
                      << a->priority << " - new priority: " << action->priority<< std::endl;

            // update queue
            a->priority = action->priority;
            std::make_heap(mActionsTodo.begin(), mActionsTodo.end(), ActionAiComp{});

            break;
        }
    }

    // add new action
    if(!found)
    {
        std::cout << "PlayerAI::AddNewAction - NEW ACTION " << action->aid
                  << " - priority: " << action->priority << std::endl;
        PushAction(action);
    }
}

void PlayerAI::AddActionsBase(Structure * s)
{
    const unsigned int numUnits = mPlayer->GetNumUnits();
    const unsigned int limitUnits = mPlayer->GetMaxUnits();

    // can't build more units -> exit
    if(numUnits >= limitUnits)
        return ;

    // define base priority
    // MAX with 0 units and MIN with limit units
    const int maxPriority = 100;
    const int priority = maxPriority * (limitUnits - numUnits) / limitUnits;

    // DECIDE UNIT TYPE
    // TODO keep into consideration faction attitude
    // (i.e.: if faction is more inclined to attack then prefer soldier over builder)
    std::vector<int> priorities(NUM_UNIT_TYPES, priority);
    std::vector<int> costs(NUM_UNIT_TYPES, 0);
    std::vector<int> relCosts(NUM_UNIT_TYPES, 0);

    // 1- exclude units that can't be built and check cost
    const int multEnergy = 1;
    const int multMaterial = 2;
    const int multBlobs = 10;
    const int multDiamonds = 20;
    int maxCost = 0;
    int validUnits = 0;

    for(unsigned int i = 0; i < NUM_UNIT_TYPES; ++i)
    {
        const ObjectData & data = mPlayer->GetAvailableUnit(static_cast<UnitType>(i));

        if(data.objClass == OC_NULL || !mGm->CanCreateUnit(data, s, mPlayer))
        {
            priorities[i] = 0;
            continue;
        }

        // total cost
        costs[i] = data.costs[RES_ENERGY] * multEnergy + data.costs[RES_MATERIAL1] * multMaterial +
                   data.costs[RES_BLOBS] * multBlobs + data.costs[RES_DIAMONDS] * multDiamonds;


        if(costs[i] > maxCost)
            maxCost = costs[i];

        // relative cost
        int costsIncluded = 0;

        const int energy = mPlayer->GetStat(Player::ENERGY).GetIntValue();
        const int material = mPlayer->GetStat(Player::MATERIAL).GetIntValue();
        const int blobs = mPlayer->GetStat(Player::BLOBS).GetIntValue();
        const int diamonds = mPlayer->GetStat(Player::DIAMONDS).GetIntValue();

        if(energy > 0)
        {
            relCosts[i] += data.costs[RES_ENERGY] * 100 / energy;
            ++costsIncluded;
        }
        if(material > 0)
        {
            relCosts[i] += data.costs[RES_MATERIAL1] * 100 / material;
            ++costsIncluded;
        }
        if(blobs > 0)
        {
            relCosts[i] += data.costs[RES_BLOBS] * 100 / blobs;
            ++costsIncluded;
        }
        if(diamonds > 0)
        {
            relCosts[i] += data.costs[RES_DIAMONDS] * 100 / diamonds;
            ++costsIncluded;
        }

        if(costsIncluded > 0)
            relCosts[i] /= costsIncluded;

        ++validUnits;
    }

    // can't create any unit -> exit
    if(0 == validUnits)
        return ;

    // 2- apply bonuses based on existing units
    const int bonusExistingUnit = -25;

    for(unsigned int i = 0; i < mPlayer->GetNumUnits(); ++i)
    {
        Unit * u = mPlayer->GetUnit(i);
        const unsigned int typeId = u->GetUnitType();

        if(priorities[typeId] <= 0)
            continue ;

        priorities[typeId] += bonusExistingUnit;
    }

    // 3- apply bonuses based on unit type
    const int bonusCost = -15;
    const int bonusRelCost = -20;

    for(unsigned int i = 0; i < NUM_UNIT_TYPES; ++i)
    {
        if(priorities[i] <= 0)
            continue ;

        // NOTE eventually costs can be weighted on the current resources.
        // i.e.: if you have a lot of X it doesn't really matter if a unit requires 20 or 40.
        priorities[i] += bonusCost * costs[i] / maxCost;

        priorities[i] += bonusRelCost * relCosts[i] / 100;
    }

    // 4- pick highest priority
    // create action
    auto action = new ActionAINewUnit;
    action->aid = AIA_NEW_UNIT;
    action->ObjSrc = s;
    action->priority = 0;
    action->unitType = UNIT_NULL;

    // for now picking first of list when priorities are the same
    for(unsigned int i = 0; i < NUM_UNIT_TYPES; ++i)
    {
        if(priorities[i] > action->priority)
        {
            action->priority = priorities[i];
            action->unitType = static_cast<UnitType>(i);
        }
    }

    // no valid unit was found -> exit
    if(UNIT_NULL == action->unitType)
    {
        mActionsDone.push_back(action);
        return ;
    }

    // push action to the queue
    AddNewAction(action);
}

void PlayerAI::AddActionsUnit(Unit * u)
{

}

} // namespace game
