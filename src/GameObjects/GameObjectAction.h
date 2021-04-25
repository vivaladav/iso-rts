#pragma once

#include "Cell2D.h"

namespace game
{

class GameObject;

enum GameObjectActionId : unsigned int;

struct GameObjectAction
{
    GameObjectAction(GameObject * go, GameObjectActionId aid);
    GameObjectAction(GameObject * go, GameObjectActionId aid, const Cell2D & cell);
    GameObjectAction(GameObject * go, GameObject * t, GameObjectActionId aid, const Cell2D & cell);

    GameObject * obj = nullptr;
    GameObject * target = nullptr;

    Cell2D actionCell;

    GameObjectActionId actId;
};

inline GameObjectAction::GameObjectAction(GameObject * go,
                                          GameObjectActionId aid)
    : obj(go)
    , actId(aid)
{
}

inline GameObjectAction::GameObjectAction(GameObject * go,
                                          GameObjectActionId aid,
                                          const Cell2D & cell)
    : obj(go)
    , actionCell(cell)
    , actId(aid)
{
}

inline GameObjectAction::GameObjectAction(GameObject * go, GameObject * t, GameObjectActionId aid, const Cell2D & cell)
    : obj(go)
    , target(t)
    , actionCell(cell)
    , actId(aid)
{
}

} // namespace game
