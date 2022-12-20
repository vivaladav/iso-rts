#pragma once

#include "ActionAI.h"

#include <vector>

namespace game
{


class GameMap;
class Player;
class Structure;
class Unit;

struct GameMapCell;

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

    void DecideActions();

    void PushAction(ActionAI * action);
    const ActionAI * PopAction();

    void AddNewAction(ActionAI * action);

    void AddActionsBase(Structure * s);
    void AddActionsUnit(Unit * u);

private:
    std::vector<ActionAI *> mActionsTodo;
    std::vector<ActionAI *> mActionsDone;

    Player * mPlayer = nullptr;

    GameMap * mGm = nullptr;
};

inline void PlayerAI::SetGameMap(GameMap * gm) { mGm = gm; }

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
