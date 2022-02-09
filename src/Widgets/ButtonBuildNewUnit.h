#pragma once

#include "Widgets/ShortcutButton.h"

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

class ButtonBuildNewUnit : public ShortcutButton
{
public:
    ButtonBuildNewUnit(const char * shortcut, int shortcutKey,
                       sgl::sgui::Widget * parent);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

    void OnStateChanged(sgl::sgui::PushButton::VisualState state) override;

private:
    sgl::graphic::Image * mBody = nullptr;
    sgl::graphic::Text * mLabel = nullptr;
    sgl::graphic::Text * mShortcut = nullptr;
};

} // namespace game
