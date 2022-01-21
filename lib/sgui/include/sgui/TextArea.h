#pragma once

#include "sgui/Widget.h"

#include <string>
#include <vector>

namespace lib
{

namespace graphic
{
    class Font;
    class Renderable;
}

namespace sgui
{

class TextArea : public Widget
{
public:
    TextArea(int w, int h, graphic::Font * font, Widget * parent = nullptr);
    TextArea(int w, int h, const char * txt, graphic::Font * font, Widget * parent = nullptr);

    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetColor(unsigned int color);

    const std::string & GetText() const;
    void SetText(const char * txt);

private:
    void CreateText();

    void AddTextLine(const char * txt);
    void AddEmptyLine(int h);

    void HandlePositionChanged() override;

private:    
    std::vector<graphic::Renderable *> mTxtLines;

    std::string mStr;

    graphic::Font * mFont = nullptr;

    unsigned char mTxtR = 255;
    unsigned char mTxtG = 255;
    unsigned char mTxtB = 255;
    unsigned char mTxtA = 255;
};

inline const std::string & TextArea::GetText() const { return mStr; }

} // namespace sgui
} // namespace lib
