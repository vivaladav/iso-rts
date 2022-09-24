#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class ButtonPlanetMap;

class PanelPlanetActions : public sgl::sgui::Widget
{
public:
    PanelPlanetActions();

private:
    void HandlePositionChanged() override;

//    void HandleStateEnabled() override;
//    void HandleStateDisabled() override;

    void UpdatePositions();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Text * mTitle = nullptr;

    ButtonPlanetMap * mButtonExplore = nullptr;
    ButtonPlanetMap * mButtonConquer = nullptr;
    ButtonPlanetMap * mButtonSendAI = nullptr;

};

} // namespace game
