#pragma once

#include "Screen.h"

namespace lib { namespace graphic { class Image; } }

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
    lib::graphic::Image * mImg1 = nullptr;
    lib::graphic::Image * mImg2 = nullptr;
    lib::graphic::Image * mImg3 = nullptr;
};

} // namespace game
