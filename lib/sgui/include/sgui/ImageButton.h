#pragma once

#include "sgui/PushButton.h"

#include <array>

namespace lib
{

namespace graphic
{
    class Image;
    class Texture;
}

namespace sgui
{

class ImageButton : public lib::sgui::PushButton
{
public:
    ImageButton(const std::array<unsigned int, NUM_VISUAL_STATES> & texIds,
                const char * spriteFile, lib::sgui::Widget * parent);

protected:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

private:
    std::array<lib::graphic::Texture *, NUM_VISUAL_STATES> mTexs;

    lib::graphic::Image * mBody = nullptr;
};

} // namespace sgui
} // namespace lib
