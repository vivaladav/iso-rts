#pragma once

#include "Screen.h"
#include "sgl/sgui/Widget.h"

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

class PanelContentSettings;

class ScreenSettings : public Screen
{
public:
    ScreenSettings(Game * game);
    ~ScreenSettings();

    void Update(float update) override;
    void Render() override;

private:
    void CreatePanelGame(sgl::sgui::Widget * parent);
    void CreatePanelAudio(sgl::sgui::Widget * parent);
    void CreatePanelVideo(sgl::sgui::Widget * parent);
    void CreatePanelControls(sgl::sgui::Widget * parent);

private:
    enum Panel : unsigned int
    {
        GAME,
        AUDIO,
        VIDEO,
        CONTROLS,

        NUM_PANELS
    };

    sgl::graphic::Image * mBg = nullptr;

    PanelContentSettings * mPanels[NUM_PANELS];
};

} // namespace game
