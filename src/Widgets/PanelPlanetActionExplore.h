#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>

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

class SecondaryButtonPlanetMap;
class ButtonPlanetMap;

class PanelPlanetActionExplore : public sgl::sgui::Widget
{
public:
    PanelPlanetActionExplore(int money, int energy, int material);

    void AddOnButtonOkClickFunction(const std::function<void()> & f);
    void AddOnButtonCancelClickFunction(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

    void UpdatePositions();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Text * mTitle = nullptr;

    ButtonPlanetMap * mButtonOk = nullptr;
    SecondaryButtonPlanetMap * mButtonCancel = nullptr;

    sgl::graphic::Text * mTextDesc = nullptr;
    sgl::graphic::Text * mTextCost = nullptr;

    sgl::graphic::Image * mIconMoney = nullptr;
    sgl::graphic::Image * mIconEnergy = nullptr;
    sgl::graphic::Image * mIconMaterial = nullptr;

    sgl::graphic::Text * mTextMoney = nullptr;
    sgl::graphic::Text * mTextEnergy = nullptr;
    sgl::graphic::Text * mTextMaterial = nullptr;
};

} // namespace game
