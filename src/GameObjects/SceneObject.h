#pragma once

#include "GameObject.h"

namespace game
{

class SceneObject : public GameObject
{
public:
    SceneObject(GameObjectType subtype, int rows, int cols);

protected:
    void UpdateImage() override;

private:
    void SetImage();

private:
    int mSubtypeInd = 0;
};

} // namespace game
