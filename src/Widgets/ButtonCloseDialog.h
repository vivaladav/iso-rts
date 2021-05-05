#pragma once

#include <sgui/PushButton.h>

namespace lib
{
    namespace graphic
    {
        class Image;
        class Texture;
    }
}

namespace game
{

class ButtonCloseDialog : public lib::sgui::PushButton
{
public:
    ButtonCloseDialog(lib::graphic::Texture * texIcon, int w, int h,
                      lib::sgui::Widget * parent);
    ~ButtonCloseDialog();

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    lib::graphic::Image * mIcon = nullptr;
};

} // namespace game
