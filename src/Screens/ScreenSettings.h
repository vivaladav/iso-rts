#pragma once

#include "Screen.h"

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class ScreenSettings : public Screen
{
public:
    ScreenSettings(Game * game);
    ~ScreenSettings();

    void Update(float update) override;
    void Render() override;

private:
    void InitSprites();

private:
    lib::graphic::Image * mBg = nullptr;
};

} // namespace game
