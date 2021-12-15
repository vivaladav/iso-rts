#pragma once

#include <sgui/Widget.h>

namespace lib
{
    namespace graphic { class Texture; }
    namespace sgui { class Image; }
}

namespace game
{

class DigitsDisplay;
class ProgressBarResources;

class SimpleResourceDisplay : public lib::sgui::Widget
{
public:
    SimpleResourceDisplay(lib::graphic::Texture * icon, int digits, lib::sgui::Widget * parent);

    void SetValue(int val);

private:
    lib::sgui::Image * mIcon = nullptr;
    DigitsDisplay * mDigits = nullptr;
};

} // namespace game
