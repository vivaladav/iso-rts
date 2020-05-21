#pragma once

#include "Screen.h"

namespace lib { namespace graphic { class Image; } }

class ScreenTest : public Screen
{
public:
    ScreenTest(unsigned int screenId, Game * app);
    ~ScreenTest();

    void Update() override;

    void OnApplicationQuit() override;
    void OnKeyUp(const lib::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

private:
    lib::graphic::Image * mImg1 = nullptr;
    lib::graphic::Image * mImg2 = nullptr;
    lib::graphic::Image * mImg3 = nullptr;
};
