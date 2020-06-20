#pragma once

#include "sgui/Widget.h"

#include <string>

namespace lib
{

namespace graphic
{
    class Font;
    class Renderable;
}

namespace sgui
{

class Label : public Widget
{
public:
    Label(int wid, graphic::Font * font, Widget * parent = nullptr);
    Label(const char * txt, int wid, graphic::Font * font, Widget * parent = nullptr);
    ~Label();

    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    const std::string & GetText() const;
    void SetText(const char * txt);

private:
    void CreateText();

    void OnPositionChanged() override;

    void OnRender() override;

private:
    std::string mStr;

    graphic::Font * mFont = nullptr;
    graphic::Renderable * mTxt = nullptr;
};

inline const std::string & Label::GetText() const { return mStr; }

} // namespace sgui
} // namespace lib
