#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace graphic { class Texture; }
    namespace sgui { class Image; }
}

namespace game
{

class DigitsDisplay;
class ProgressBarResources;

class ResourceDisplay : public sgl::sgui::Widget
{
public:
    ResourceDisplay(sgl::graphic::Texture * icon, int digits, sgl::sgui::Widget * parent);

    void SetValue(int val);
    void SetValueMinMax(int min, int max);

private:
    sgl::sgui::Image * mIcon = nullptr;
    DigitsDisplay * mDigits = nullptr;
    ProgressBarResources * mBar = nullptr;
};

} // namespace game
