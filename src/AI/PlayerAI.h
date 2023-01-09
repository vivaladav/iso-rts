#pragma once

#include "ActionAI.h"

#include <vector>

namespace game
{

class GameMap;
class GameObject;
class Player;
class Structure;
class Unit;

struct GameMapCell;

enum ResourceType : unsigned int;

class PlayerAI
{
public:
    PlayerAI(Player * player);
    ~PlayerAI();

    void SetGameMap(GameMap * gm);

    void Update(float delta);

    const ActionAI * GetNextActionTodo();

    void HandleObjectDestroyed(GameObject * obj);

    void RegisterActionInProgress(const ActionAI * action);
    void SetActionDone(const ActionAI * action);

    Player * GetPlayer();

private:
    void ClearActionsDone();
    void ClearActionsTodo();

    void HandleObjectDestroyedInTodo(GameObject * obj);
    void HandleObjectDestroyedInDoing(GameObject * obj);

    void PrepareData();

    void UpdatePriorityRange();

    void DecideActions();

    void PushAction(ActionAI * action);
    const ActionAI * PopAction();

    void AddNewAction(ActionAI * action);

    void AddActionsBase(Structure * s);
    void AddActionsUnit(Unit * u);
    void AddActionUnitConnectStructure(Unit * u);
    void AddActionUnitConquestResGen(Unit * u, ResourceType type);

    bool IsSimilarActionInProgress(AIActionType type) const;

private:
    std::vector<ActionAI *> mActionsTodo;
    std::vector<const ActionAI *> mActionsDoing;
    std::vector<const ActionAI *> mActionsDone;

    // shared data,
    std::vector<GameObject *> mResGenerators;
    std::vector<GameObject *> mStructures;

    Player * mPlayer = nullptr;

    GameMap * mGm = nullptr;

    int mMinPriority = 0;
};

inline void PlayerAI::SetGameMap(GameMap * gm) { mGm = gm; }

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

inline void PlayerAI::RegisterActionInProgress(const ActionAI * action)
{
    mActionsDoing.push_back(action);
}

} // namespace game
