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

    const ActionAI * GetNextAction();

    Player * GetPlayer();

private:
    void ClearActionsDone();
    void ClearActionsTodo();

    void PrepareData();

    void DecideActions();

    void PushAction(ActionAI * action);
    const ActionAI * PopAction();

    void AddNewAction(ActionAI * action);

    void AddActionsBase(Structure * s);
    void AddActionsUnit(Unit * u);
    void AddActionUnitConquestResGen(Unit * u, ResourceType type);

    int ApproxDistance(GameObject * obj1, GameObject * obj2) const;

private:
    std::vector<ActionAI *> mActionsTodo;
    std::vector<ActionAI *> mActionsDone;

    // shared data
    std::vector<GameObject *> mResGenerators;

    Player * mPlayer = nullptr;

    GameMap * mGm = nullptr;
};

inline void PlayerAI::SetGameMap(GameMap * gm) { mGm = gm; }

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
