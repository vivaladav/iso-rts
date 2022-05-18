#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }

    namespace sgui { class Label; }
}

namespace game
{

class PanelPlanetInfo : public sgl::sgui::Widget
{
public:
    PanelPlanetInfo();

    void SetResourceValue(unsigned int res, unsigned int value);

private:
    void HandlePositionChanged() override;

    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void UpdatePositions();

private:
    sgl::graphic::Text * mTitle = nullptr;
    sgl::graphic::Text * mHeaderSize = nullptr;
    sgl::graphic::Text * mHeaderStatus = nullptr;
    sgl::graphic::Text * mHeaderValue = nullptr;
    sgl::graphic::Text * mHeaderOccupier = nullptr;

    sgl::graphic::Text * mLabelSize = nullptr;
    sgl::graphic::Text * mLabelStatus = nullptr;
    sgl::graphic::Text * mLabelOccupier = nullptr;
    sgl::graphic::Image * mBarValue = nullptr;

    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
