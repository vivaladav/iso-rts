#pragma once

#include "Cell2D.h"

namespace game
{

class GameObject;

struct GameObjectAction
{
    GameObjectAction(GameObject * go, const Cell2D & cell);

    GameObject * obj = nullptr;

    Cell2D actionCell;
};

inline GameObjectAction::GameObjectAction(GameObject * go, const Cell2D & cell)
    : obj(go)
    , actionCell(cell)
{
}

} // namespace game
