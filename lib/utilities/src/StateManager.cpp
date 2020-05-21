#include "utilities/StateManager.h"

#include "utilities/State.h"

#include <iostream>

namespace lib
{
namespace utilities
{

State dummy(0xFFFFFFFF);

StateManager::StateManager()
    : mActive(&dummy)
{
}

StateManager::~StateManager()
{
    mActive = nullptr;

    for(auto & s : mStates)
        delete s.second;
}

bool StateManager::AddState(int stateId, State * state)
{
    // no state
    if(!state)
        return false;

    auto res = mStates.find(stateId);

    // state is already registered
    if(res != mStates.end())
    {
        std::cerr << "StateManager::AddState - STATE " << stateId << " ALREADY ADDED" << std::endl;
        return false;
    }

    // add state
    state->mManager = this;
    mStates.emplace(stateId, state);

    return true;
}

State * StateManager::RemoveState(int stateId)
{
    auto res = mStates.find(stateId);

    if(res != mStates.end())
    {
        mStates.erase(res);

        // if state is active replace it with the dummy one
        if(res->second == mActive)
            mActive = &dummy;

        return res->second;
    }

    return nullptr;
}

void StateManager::RemoveAndDestroyState(int stateId)
{
    State * res = RemoveState(stateId);
    delete res;
}

bool StateManager::RequestNextActiveState(int stateId)
{
    auto res = mStates.find(stateId);

    // state not found
    if(res == mStates.end())
        return false;

    // requesting same state as already active
    if(res->second == mActive)
        return false;

    // schedule state to be next active
    mNext = res->second;

    return true;
}

void StateManager::UpdateActive()
{
    if(!mNext)
        return;

    mActive->OnInactive();

    mActive = mNext;
    mNext = nullptr;

    mActive->OnActive();
}

} // namespace utilities
} // namespace lib
