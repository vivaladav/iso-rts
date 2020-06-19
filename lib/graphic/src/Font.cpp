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
    : mFile(file)
    , mSize(size)
    , mStyle(NORMAL)
{
    CreateSysFont();
}

Font::~Font()
{
    for(const auto & it : mSysFonts)
        TTF_CloseFont(it.second);
}

void Font::SetStyle(int s)
{
    // same style -> nothing to do
    if(s == mStyle)
        return ;

    mStyle = s;

    auto res = mSysFonts.find(mStyle);

    if(res == mSysFonts.end())
        CreateSysFont();
    else
        mActiveSysFont = res->second;
}

void Font::CreateSysFont()
{
    mActiveSysFont = TTF_OpenFont(mFile.c_str(), mSize);

    if(!mActiveSysFont)
    {
        std::cout << "TTF_OpenFont ERROR: " << TTF_GetError() << std::endl;
        return ;
    }

    if(mStyle != NORMAL)
        TTF_SetFontStyle(mActiveSysFont, mStyle);

    mSysFonts.emplace(mStyle, mActiveSysFont);
}

} // namespace graphic
} // namespace lib
