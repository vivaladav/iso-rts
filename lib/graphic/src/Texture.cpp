#include "graphic/Texture.h"

#include "graphic/Renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cassert>
#include <iostream>

namespace lib
{
namespace graphic
{

Texture::Texture(const char * file)
{
    assert(file);

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


} // namespace graphic
} // namespace lib
