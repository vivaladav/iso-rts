#pragma once

#include "ActionAI.h"

#include <vector>

namespace game
{


class GameMap;
class Player;
class Structure;

struct GameMapCell;

class PlayerAI
{
public:
    PlayerAI(Player * player);

    void SetGameMap(GameMap * gm);

    void DecideActions();

    ActionAI GetNextAction();

    Player * GetPlayer();

private:
    void PushAction(const ActionAI & action);
    ActionAI PopAction();

    void AddNewAction(const ActionAI & action);

    void AddActionsBase(Structure * s);

private:
    std::vector<ActionAI> mActions;

    Player * mPlayer = nullptr;

    GameMap * mGm = nullptr;
};

inline void PlayerAI::SetGameMap(GameMap * gm) { mGm = gm; }

inline Player * PlayerAI::GetPlayer() { return mPlayer; }

} // namespace game
