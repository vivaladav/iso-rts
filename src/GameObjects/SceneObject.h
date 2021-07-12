#pragma once

#include "GameObject.h"

namespace game
{

class SceneObject : public GameObject
{
public:
    SceneObject(GameObjectType subtype, int rows, int cols);

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();

private:
    int mSubtypeInd = 0;
};

} // namespace game
