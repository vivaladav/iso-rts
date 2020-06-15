#include "graphic/Text.h"

#include "graphic/Font.h"
#include "graphic/Renderer.h"
#include "graphic/Texture.h"
#include "graphic/TextureManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cassert>
#include <iostream>

namespace lib
{
namespace graphic
{

Text::Text(const char * text, Font * font)
{
    assert(text);
    assert(font);

    // text is always white as color is set later
    const SDL_Color c = { 255, 255, 255, 255};

    SDL_Surface * surf = TTF_RenderUTF8_Blended(font->mSysFont, text, c);

    if(!surf)
    {
       std::cout << "TTF Render ERROR: " << TTF_GetError() << std::endl;
       return ;
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
