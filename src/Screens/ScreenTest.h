#pragma once

#include "Screen.h"

#include <vector>

namespace lib
{
    namespace graphic { class TexturedRenderable; }
}

namespace game
{

class ScreenTest : public Screen
{
public:
    ScreenTest(Game * game);
    ~ScreenTest();

    void Update() override;
    void Render() override;

    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

private:
    void TestSGui();

private:
    std::vector<lib::graphic::TexturedRenderable *> mRenderables;
};

} // namespace game
