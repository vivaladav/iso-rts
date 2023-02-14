#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class AbstractButtonsGroup; }
}

namespace game
{

class ButtonCloseSettings;
class Game;
class PanelContentSettings;

class DialogSettings : public sgl::sgui::Widget
{
public:
    DialogSettings();
    ~DialogSettings();

    void AddOnCloseClickedFunction(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

    void SetPositions();

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

    private:
    sgl::sgui::AbstractButtonsGroup * mGroupButtons = nullptr;

    PanelContentSettings * mPanels[NUM_PANELS];

    sgl::graphic::Image * mBg = nullptr;

    ButtonCloseSettings * mButtonBack = nullptr;
};

} // namespace game
