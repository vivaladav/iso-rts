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
    Label(const char * txt, int wid, Widget * parent = nullptr);

private:
    graphic::Text * mTxt = nullptr;
};

} // namespace sgui
} // namespace lib
