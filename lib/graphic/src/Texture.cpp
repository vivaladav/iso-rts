#include "graphic/Texture.h"

#include "graphic/GraphicConstants.h"
#include "graphic/Renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cassert>
#include <iostream>

namespace lib
{
namespace graphic
{

Texture::Texture(const char * file, TextureQuality q)
{
    assert(file);

    // sest quality BEFORE creating a new texture
    SetTextureQuality(q);

    SDL_Surface * img = IMG_Load(file);

    if(!img)
    {
        std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
        return ;
    }

    mData = SDL_CreateTextureFromSurface(Renderer::Instance()->mSysRenderer, img);

    mW = img->w;
    mH = img->h;

    SDL_FreeSurface(img);
}

Texture::~Texture()
{
    SDL_DestroyTexture(mData);
}

void Texture::SetTextureQuality(TextureQuality q)
{
    if(TextureQuality::BEST == q)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");    // anisotropic
    else if(TextureQuality::GOOD == q)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");    // linear
    else
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");    // nearest pixel
}

} // namespace graphic
} // namespace lib

