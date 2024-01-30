#pragma once

#include "Screen.h"

#include <vector>

namespace sgl
{
    namespace core { class Timer; }
    namespace graphic { class TexturedRenderable; }
}

namespace game
{

class GameProgressBar;

class ScreenTest : public Screen
{
public:
    ScreenTest(Game * game);
    ~ScreenTest();

    void OnKeyUp(sgl::core::KeyboardEvent & event) override;

    void Update(float delta) override;
    void Render() override;

    void OnMouseButtonUp(sgl::core::MouseButtonEvent & event) override;

private:
    void TestSGui();
    void TestSprite();
    void TestRotation();
    void TestTimer();
    void TestAudio();
    void TestUtilities();

private:
    std::vector<sgl::graphic::TexturedRenderable *> mRenderables;

    sgl::core::Timer * mTimer1 = nullptr;
    sgl::core::Timer * mTimer2 = nullptr;

    GameProgressBar * mPb0 = nullptr;
    GameProgressBar * mPb1 = nullptr;
    GameProgressBar * mPb2 = nullptr;
    float mTimerPb = 0.f;
};

} // namespace game
