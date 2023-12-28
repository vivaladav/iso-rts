#pragma once

#include "GameObject.h"

namespace game
{

class Trees : public GameObject
{
public:
    Trees(GameObjectVariantId var);

    void Update(float delta) override;

private:
    void SpawnTree(int r0, int c0);

    void UpdateGraphics() override;

    void SetImage();

    void SetObjColors();

private:
    int mLevel = 0;
    int mNumTrees = 1;
    float mTimeChange = 0.f;
    float mTimerChange = 0.f;
};

} // namespace game
