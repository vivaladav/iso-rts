#pragma once

#include "Widgets/ShortcutButton.h"

namespace lib
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class ButtonBuildNewUnit : public ShortcutButton
{
public:
    ButtonBuildNewUnit(const char * shortcut, int shortcutKey,
                       lib::sgui::Widget * parent);
    ~ButtonBuildNewUnit();

protected:
    void SetElements(VisualState state) override;

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    lib::graphic::Image * mBody = nullptr;
    lib::graphic::Text * mLabel = nullptr;
    lib::graphic::Text * mShortcut = nullptr;
};

} // namespace game
