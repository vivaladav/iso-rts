#pragma once

#include "sgui/Widget.h"

namespace lib
{

namespace graphic
{
    class Font;
    class Text;
}

namespace sgui
{

class Label : public Widget
{
public:
    Label(const char * txt, int wid, graphic::Font * font, Widget * parent = nullptr);

private:
    void OnPositionChanged() override;

    void OnRender() override;

private:
    graphic::Font * mFont = nullptr;
    graphic::Text * mTxt = nullptr;
};

} // namespace sgui
} // namespace lib
