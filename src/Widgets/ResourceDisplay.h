#pragma once

#include <sgl/sgui/Widget.h>

namespace lib
{
    namespace graphic { class Texture; }
    namespace sgui { class Image; }
}

namespace game
{

class DigitsDisplay;
class ProgressBarResources;

class ResourceDisplay : public lib::sgui::Widget
{
public:
    ResourceDisplay(lib::graphic::Texture * icon, int digits, lib::sgui::Widget * parent);

    void SetValue(int val);
    void SetValueMinMax(int min, int max);

private:
    lib::sgui::Image * mIcon = nullptr;
    DigitsDisplay * mDigits = nullptr;
    ProgressBarResources * mBar = nullptr;
};

} // namespace game
