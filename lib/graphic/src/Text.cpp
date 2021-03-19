#include "graphic/Text.h"

#include "graphic/Font.h"
#include "graphic/Renderer.h"
#include "graphic/Texture.h"
#include "graphic/TextureManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cassert>
#include <iostream>
#include <string>

namespace lib
{
namespace graphic
{

Text::Text(const char * text, Font * font, bool trim)
{
    assert(text);
    assert(font);

    // text is always created white as color is set later
    const SDL_Color white = { 255, 255, 255, 255};

    TTF_Font * f = font->GetSysFont();
    SDL_Surface * surf = TTF_RenderUTF8_Blended(f, text, white);

    if(!surf)
    {
       std::cout << "TTF Render ERROR: " << TTF_GetError() << std::endl;
       return ;
    }

    // define top and bottom padding
    const std::string str(text);
    const int strLen = str.length();

    const int height = TTF_FontHeight(f);
    const int ascent = TTF_FontAscent(f);
    mPaddingBottom = height - ascent;

    mPaddingTop = height;

    for(int i = 0; i < strLen; ++i)
    {
        int miny = 0;
        int maxy = 0;
        TTF_GlyphMetrics(f, str[i], nullptr, nullptr, &miny, &maxy, nullptr);

        const int top = height - mPaddingBottom - (maxy - miny);

        if(top < mPaddingTop)
            mPaddingTop = top;
    }

    // trim text to remove upper and lower margin
    if(trim)
    {
        const int trimH = height - mPaddingTop - mPaddingBottom;

        const SDL_PixelFormat * pf = surf->format;
        SDL_Surface * trimSurf = SDL_CreateRGBSurface(0, surf->w, trimH, pf->BitsPerPixel,
                                                      pf->Rmask, pf->Gmask, pf->Bmask, pf->Amask);

        // set blend mode of source to NONE to have exact pixel copy
        SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_NONE);

        const SDL_Rect srcRect = { 0, mPaddingTop, surf->w, trimH };
        SDL_BlitSurface(surf, &srcRect, trimSurf, nullptr);

        SDL_FreeSurface(surf);
        surf = trimSurf;

        // reset padding
        mPaddingTop = 0;
        mPaddingBottom = 0;
    }

    const TextureQuality tq = TextureManager::Instance()->GetNewTextureQuality();

    Texture * tex = new Texture(surf, tq);
    SetTexture(tex);

    SDL_FreeSurface(surf);
}

Text::~Text()
{
    DeleteTexture();
}

} // namespace graphic
} // namespace lib
