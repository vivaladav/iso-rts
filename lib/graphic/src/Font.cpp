#include "graphic/Font.h"

#include "core/Size.h"

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

int Font::GetStyle() const
{
    return TTF_GetFontStyle(mSysFont);
}

core::Sized Font::GetTextSize(const char * text)
{
    core::Sized size;

    // reset size in case of error
    if(TTF_SizeUTF8(mSysFont, text, &(size.w), &(size.h)) != 0)
    {
        size.w = 0;
        size.h = 0;
    }

    return size;
}

Font::Font(const char * file, int size, int style)
    : mSize(size)
{
    mSysFont = TTF_OpenFont(file, mSize);

    if(!mSysFont)
    {
        std::cout << "TTF_OpenFont ERROR: " << TTF_GetError() << std::endl;
        return ;
    }

    if(style != NORMAL)
        TTF_SetFontStyle(mSysFont, style);
}

Font::~Font()
{
    TTF_CloseFont(mSysFont);
}

} // namespace graphic
} // namespace lib
