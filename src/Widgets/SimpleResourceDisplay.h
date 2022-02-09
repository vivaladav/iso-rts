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

class SimpleResourceDisplay : public sgl::sgui::Widget
{
public:
    SimpleResourceDisplay(sgl::graphic::Texture * icon, int digits, sgl::sgui::Widget * parent);

    void SetValue(int val);

private:
    sgl::sgui::Image * mIcon = nullptr;
    DigitsDisplay * mDigits = nullptr;
};

} // namespace game
