#pragma once

#include "Screen.h"

namespace sgl { namespace graphic { class Image; } }

namespace game
{

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(Game * game);
    ~ScreenMainMenu();

    void Update(float update) override;
    void Render() override;

private:
    void OnApplicationQuit(sgl::core::ApplicationEvent & event) override;

private:
    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
