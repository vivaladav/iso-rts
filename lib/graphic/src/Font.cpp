#include "graphic/Font.h"

#include <SDL2/SDL_ttf.h>

#include <iostream>

namespace lib
{
namespace graphic
{

const int Font::NORMAL          = TTF_STYLE_NORMAL;
const int Font::BOLD            = TTF_STYLE_BOLD;
const int Font::ITALIC          = TTF_STYLE_ITALIC;
const int Font::STRIKETHROUGH   = TTF_STYLE_STRIKETHROUGH;
const int Font::UNDERLINE       = TTF_STYLE_UNDERLINE;

Font::Font(const char * file, int size)
    : mSize(size)
    , mStyle(NORMAL)
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

void Font::SetStyle(int s)
{
    // same style or font not valid -> nothing to do
    if(s == mStyle || !mSysFont)
        return ;

    mStyle = s;

    TTF_SetFontStyle(mSysFont, mStyle);
}

} // namespace graphic
} // namespace lib
