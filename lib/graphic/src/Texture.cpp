#include "graphic/Texture.h"

#include "graphic/GraphicConstants.h"
#include "graphic/Renderer.h"
#include "graphic/TextureData.h"

#include <SDL2/SDL.h>

namespace lib
{
namespace graphic
{

Texture::Texture()
    : mSrcRect(new SDL_Rect{0, 0, 0, 0})
{
}

Texture::Texture(SDL_Surface * data, TextureQuality q)
    : mSrcRect(new SDL_Rect)
{
    mTexData = std::make_shared<TextureData>(data, q);

    ResetSourceRect();
}

Texture::Texture(const char * file, TextureQuality q)
    : mSrcRect(new SDL_Rect)
{
    mTexData = mTexData = std::make_shared<TextureData>(file, q);

    ResetSourceRect();
}

Texture::Texture(const std::shared_ptr<TextureData> & data)
    : mTexData(data)
    , mSrcRect(new SDL_Rect)
{
    ResetSourceRect();
}

Texture::~Texture()
{
    delete mSrcRect;
}

void Texture::SetData(const std::shared_ptr<TextureData> & data)
{
    mTexData = data;
}

int Texture::GetWidth() const { return mSrcRect->w; }
int Texture::GetHeight() const { return mSrcRect->h; }

void Texture::ResetSourceRect()
{
    mSrcRect->x = 0;
    mSrcRect->y = 0;
    mSrcRect->w = mTexData->GetWidth();
    mSrcRect->h = mTexData->GetHeight();
}

void Texture::SetSourceRect(int x, int y, int w, int h)
{
    mSrcRect->x = x;
    mSrcRect->y = y;
    mSrcRect->w = w;
    mSrcRect->h = h;
}

void Texture::SetSourceRect(const core::Rectd & rect)
{
    mSrcRect->x = rect.x;
    mSrcRect->y = rect.y;
    mSrcRect->w = rect.w;
    mSrcRect->h = rect.h;
}

void Texture::SetAlpha(unsigned char a)
{
    mTexData->SetAlpha(a);
}

void Texture::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mTexData->SetColor(r, g, b, a);
}

} // namespace graphic
} // namespace lib

