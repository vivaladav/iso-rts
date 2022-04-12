#pragma once

#include <sgl/sgui/Widget.h>

#include <vector>

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

class PanelPlanetResources : public sgl::sgui::Widget
{
public:
    PanelPlanetResources();

    void SetResourceValue(unsigned int res, unsigned int value);

private:
    void HandlePositionChanged() override;

    void HandleStateEnabled() override;
    void HandleStateDisabled() override;

    void UpdatePositions();

private:
    std::vector<sgl::graphic::Image *> mIcons;
    std::vector<sgl::graphic::Image *> mBars;
    std::vector<int> mBarsValue;

    sgl::graphic::Image * mBg = nullptr;
};

} // namespace game
