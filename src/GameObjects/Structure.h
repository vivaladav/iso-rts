#pragma once

#include "GameObject.h"

#include <string>
#include <unordered_map>

namespace game
{

class Structure : public GameObject
{
public:
    Structure(GameObjectTypeId type, int rows, int cols);
};

inline Structure::Structure(GameObjectTypeId type, int rows, int cols)
    : GameObject(type, rows, cols)
{
    SetStructure(true);
    SetStatic(true);
}

} // namespace game
