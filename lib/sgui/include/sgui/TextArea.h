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
    enum Alignment : unsigned int
    {
        // HORIZONTAL ALIGNMENT
        ALIGN_H_LEFT,
        ALIGN_H_CENTER,
        ALIGN_H_RIGHT,
        // VERTICAL ALIGNMENT
        ALIGN_V_TOP,
        ALIGN_V_CENTER,
        ALIGN_V_BOTTOM,
    };

public:
    TextArea(int w, int h, graphic::Font * font, Widget * parent = nullptr);
    TextArea(int w, int h, const char * txt, graphic::Font * font, Widget * parent = nullptr);

    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetColor(unsigned int color);

    void setTextAlignment(Alignment horiz, Alignment vert);
    void setTextAlignmentHorizontal(Alignment horiz);
    void setTextAlignmentVertical(Alignment vert);

    void ClearText();
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

    Alignment mAlignH = ALIGN_H_LEFT;
    Alignment mAlignV = ALIGN_V_TOP;

    unsigned char mTxtR = 255;
    unsigned char mTxtG = 255;
    unsigned char mTxtB = 255;
    unsigned char mTxtA = 255;
};

inline void TextArea::setTextAlignment(Alignment horiz, Alignment vert)
{
    if(mAlignH == horiz && mAlignV == vert)
        return ;

    mAlignH = horiz;
    mAlignV = vert;

    HandlePositionChanged();
}

inline void TextArea::setTextAlignmentHorizontal(Alignment horiz)
{
    if(mAlignH == horiz)
        return ;

    mAlignH = horiz;

    HandlePositionChanged();
}

inline void TextArea::setTextAlignmentVertical(Alignment vert)
{
    if(mAlignV == vert)
        return ;

    mAlignV = vert;

    HandlePositionChanged();
}

inline const std::string & TextArea::GetText() const { return mStr; }

} // namespace sgui
} // namespace lib
