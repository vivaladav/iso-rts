#pragma once

#include <sgl/sgui/Slider.h>

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{


// ==== COMBO BOX ====
class TestSliderH : public sgl::sgui::Slider
{

public:
    TestSliderH(sgl::sgui::Widget * parent);

private:
    void OnStateChanged(sgl::sgui::Slider::VisualState state) override;

    void HandlePositionChanged() override;

    void HandleValueChanged(int val) override;

    void UpdateGraphics(sgl::sgui::Slider::VisualState state);
    void UpdatePositions();
};

} // namespace game
