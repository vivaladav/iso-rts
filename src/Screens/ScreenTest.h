#pragma once

#include "Screen.h"

#include <vector>

namespace lib
{
    namespace graphic { class TexturedRenderable; }
}

namespace game
{

class CellProgressBar;

class ScreenTest : public Screen
{
public:
    ScreenTest(Game * game);
    ~ScreenTest();

    void Update(float delta) override;
    void Render() override;

    void OnMouseButtonUp(lib::core::MouseButtonEvent & event) override;

private:
    void TestSGui();

private:
    std::vector<lib::graphic::TexturedRenderable *> mRenderables;

    CellProgressBar * mPb = nullptr;
    float mTimerPb = 0.f;
};

} // namespace game
