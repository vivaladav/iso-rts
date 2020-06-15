#include "graphic/Font.h"

#include <SDL2/SDL_ttf.h>

#include <iostream>

namespace lib
{
namespace graphic
{

Font::Font(const char * file, int size)
    : mSize(size)
{
    mSysFont = TTF_OpenFont(file, size);

    if(!mSysFont)
    {
        mSize = 0;

        std::cout << "TTF_OpenFont ERROR: " << TTF_GetError() << std::endl;
    }
}

Font::~Font()
{
    if(mSysFont)
        TTF_CloseFont(mSysFont);
}

void Font::SetBold(bool val)
{
    if(val != mBold)
    {
        mBold = val;
        UpdateStyle();
    }
}

void Font::SetItalic(bool val)
{
    if(val != mItalic)
    {
        mItalic = val;
        UpdateStyle();
    }
}

void Font::SetStrikethrough(bool val)
{
    if(val != mStrikethrough)
    {
        mStrikethrough = val;
        UpdateStyle();
    }
}

void Font::SetUnderline(bool val)
{
    if(val != mUnderline)
    {
        mUnderline = val;
        UpdateStyle();
    }
}

void Font::UpdateStyle()
{
    // do nothing if font is not valid
    if(!mSysFont)
        return ;

    int style = TTF_STYLE_NORMAL;

    if(mBold)
        style |= TTF_STYLE_BOLD;
    if(mItalic)
        style |= TTF_STYLE_ITALIC;
    if(mStrikethrough)
        style |= TTF_STYLE_STRIKETHROUGH;
    if(mUnderline)
        style |= TTF_STYLE_UNDERLINE;

    TTF_SetFontStyle(mSysFont, style);
}

} // namespace graphic
} // namespace lib
