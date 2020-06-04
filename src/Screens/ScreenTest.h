#pragma once

#include "Screen.h"

#include <core/event/MouseEventListener.h>

namespace lib { namespace graphic { class Image; } }

namespace game
{

class ScreenTest : public Screen, public lib::core::MouseEventListener
{
public:
    ScreenTest(Game * game);
    ~ScreenTest();

    void Update() override;
    void Render() override;

    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

private:
    void OnActive() override;
    void OnInactive() override;

private:
    lib::graphic::Image * mImg1 = nullptr;
    lib::graphic::Image * mImg2 = nullptr;
    lib::graphic::Image * mImg3 = nullptr;
};

} // namespace game
