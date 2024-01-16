#pragma once

#include "Cell2D.h"

namespace game
{

class GameMapProgressBar;
class GameObject;

enum GameObjectActionId : unsigned int;

struct GameObjectAction
{
    GameObjectAction(GameObject * go, GameObjectActionId aid);
    GameObjectAction(GameObject * go, GameObjectActionId aid, const Cell2D & cell, GameMapProgressBar * pb);
    GameObjectAction(GameObject * go, GameObject * t, GameObjectActionId aid, const Cell2D & cell, GameMapProgressBar * pb);

    GameObject * obj = nullptr;
    GameObject * target = nullptr;

    GameMapProgressBar * progressBar = nullptr;

    Cell2D actionCell;

    GameObjectActionId actId;
};

inline GameObjectAction::GameObjectAction(GameObject * go,
                                          GameObjectActionId aid)
    : obj(go)
    , actId(aid)
{
}

inline GameObjectAction::GameObjectAction(GameObject * go, GameObjectActionId aid,
                                          const Cell2D & cell, GameMapProgressBar * pb)
    : obj(go)
    , progressBar(pb)
    , actionCell(cell)
    , actId(aid)
{
}

inline GameObjectAction::GameObjectAction(GameObject * go, GameObject * t, GameObjectActionId aid,
                                          const Cell2D & cell, GameMapProgressBar * pb)
    : obj(go)
    , target(t)
    , progressBar(pb)
    , actionCell(cell)
    , actId(aid)
{
}

} // namespace game
