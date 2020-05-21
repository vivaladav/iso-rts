#pragma once

namespace lib
{
namespace utilities
{

class StateManager;

class State
{
public:
    State(int stateId);
    virtual ~State();

    unsigned int GetStateId() const;

    virtual void OnActive();
    virtual void OnInactive();

protected:
    StateManager * GetManager() const;

private:
    StateManager * mManager = nullptr;

    unsigned int mId = 0;

    // manager can set the pointer to itself
    friend class StateManager;
};

inline State::State(int stateId) : mId(stateId) { }
inline State::~State() { }

inline unsigned int State::GetStateId() const { return mId; }

StateManager * State::GetManager() const { return mManager; }

} // namespace utilities
} // namespace lib
