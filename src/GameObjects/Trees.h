#pragma once

#include "GameObject.h"

namespace game
{

class Trees : public GameObject
{
public:
    Trees(GameObjectType subtype, int rows, int cols);

    void Update(float delta) override;

private:
    void UpdateGraphics() override;

    void SetImage();

    void SetObjColors();

private:
    int mSubtype = 0;

    int mLevel = 0;
    int mNumTrees = 1;
    int mVariant = 0;
    float mTimeChange = 0.f;
    float mTimerChange = 0.f;
};

} // namespace game