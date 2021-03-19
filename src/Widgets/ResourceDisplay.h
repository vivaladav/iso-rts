#pragma once

#include <sgui/Widget.h>

namespace lib { namespace sgui { class Image; } }

namespace game
{

class DigitsDisplay;
class ProgressBarResources;

class ResourceDisplay : public lib::sgui::Widget
{
public:
    ResourceDisplay(const char * icon, lib::sgui::Widget * parent);

    void SetValue(int val);
    void SetValueMinMax(int min, int max);

private:
    lib::sgui::Image * mIcon = nullptr;
    DigitsDisplay * mDigits = nullptr;
    ProgressBarResources * mBar = nullptr;
};

} // namespace game
