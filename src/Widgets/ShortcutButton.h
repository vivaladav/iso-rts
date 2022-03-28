#pragma once

#include <sgl/sgui/AbstractButton.h>

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

class ShortcutButton : public sgl::sgui::AbstractButton
{
public:
    ShortcutButton(int shortcutKey, sgl::sgui::Widget * parent);

protected:
    void HandleKeyDown(sgl::core::KeyboardEvent & event) override;
    void HandleKeyUp(sgl::core::KeyboardEvent & event) override;

private:
    int mShortcutKey = -1;
};

} // namespace game
