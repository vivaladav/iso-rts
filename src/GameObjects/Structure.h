#pragma once

#include "GameObject.h"

namespace game
{

class Structure : public GameObject
{
public:
    Structure(GameObjectTypeId type, GameObjectCategoryId cat, int rows, int cols);
};

inline Structure::Structure(GameObjectTypeId type, GameObjectCategoryId cat, int rows, int cols)
    : GameObject(type, cat, rows, cols)
{
    SetStructure(true);
    SetStatic(true);
}

} // namespace game
