#pragma once

#include "GameObject.h"

namespace game
{

class Wall : public GameObject
{
public:
    Wall(GameObjectType subtype, int rows, int cols);

    void SetWallType(GameObjectType type);

protected:
    void UpdateImage() override;

private:
    void SetImage();

private:
    int mSubtypeInd = 0;
};

} // namespace game
