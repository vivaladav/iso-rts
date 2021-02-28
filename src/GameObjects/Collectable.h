#pragma once

#include "GameObject.h"

namespace game
{

class CollectableGenerator;

class Collectable : public GameObject
{
public:
    Collectable(GameObjectType type, int rows, int cols);

    void SetGenerator(CollectableGenerator * gen);

    void Collected();

private:
    CollectableGenerator * mGen = nullptr;
};

inline Collectable::Collectable(GameObjectType type, int rows, int cols) : GameObject(type, -1, rows, cols)
{
}

inline void Collectable::SetGenerator(CollectableGenerator * gen) { mGen = gen; }

} // namespace game
