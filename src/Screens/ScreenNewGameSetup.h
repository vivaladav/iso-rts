#pragma once

#include "Screen.h"

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

class ScreenNewGameSetup : public Screen
{
public:
    ScreenNewGameSetup(Game * game);
    ~ScreenNewGameSetup();

    void Update(float delta) override;
    void Render() override;

private:
    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
