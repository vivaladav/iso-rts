#include "graphic/TextureData.h"

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

TextureData::TextureData(SDL_Surface * data, TextureQuality q)
{
    assert(data);

    // sest quality BEFORE creating a new texture
    SetTextureQuality(q);

    mData = SDL_CreateTextureFromSurface(Renderer::Instance()->mSysRenderer, data);

    mWidth = data->w;
    mHeight = data->h;
}

TextureData::TextureData(const char * file, TextureQuality q)
    : mFilePath(file)
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

    mWidth = img->w;
    mHeight = img->h;

    SDL_FreeSurface(img);
}

TextureData::~TextureData()
{
    SDL_DestroyTexture(mData);
}

void TextureData::SetAlpha(unsigned char a)
{
    SDL_SetTextureAlphaMod(mData, a);
}

void TextureData::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SDL_SetTextureColorMod(mData, r, g, b);
    SDL_SetTextureAlphaMod(mData, a);
}

void TextureData::SetTextureQuality(TextureQuality q)
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

