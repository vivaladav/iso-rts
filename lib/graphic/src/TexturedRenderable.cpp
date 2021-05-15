#include "graphic/TexturedRenderable.h"

#include "graphic/Camera.h"
#include "graphic/Renderer.h"
#include "graphic/Texture.h"
#include "graphic/TextureData.h"

#include <SDL2/SDL.h>

#include <cmath>

namespace lib
{
namespace graphic
{

const int TexturedRenderable::FLIP_NO    = SDL_FLIP_NONE;
const int TexturedRenderable::FLIP_HORIZ = SDL_FLIP_HORIZONTAL;
const int TexturedRenderable::FLIP_VERT  = SDL_FLIP_VERTICAL;

TexturedRenderable::TexturedRenderable(Texture * tex)
{
    SetTexture(tex);
}

void TexturedRenderable::Render()
{
    mTex->SetColor(mR, mG, mB, mA);

    SDL_Renderer * r = Renderer::Instance()->mSysRenderer;
    const SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(mFlip);

    const SDL_Rect rectDest
    {
        mRect->x - mCamera->GetX(),
        mRect->y - mCamera->GetY(),
        mRect->w,
        mRect->h
    };

    SDL_RenderCopyEx(r, mTex->mTexData->mData, mTex->mSrcRect, &rectDest, mRot, nullptr, flip);
}

void TexturedRenderable::SetTexture(Texture * tex)
{
    mTex = tex;

    mRect->w = tex->GetWidth();
    mRect->h = tex->GetHeight();

    mOrigW = mRect->w;
    mOrigH = mRect->h;

    UpdateRotatedSize();
}

void TexturedRenderable::DeleteTexture()
{
    delete mTex;
    mTex = nullptr;
}

void TexturedRenderable::UpdateRotatedSize()
{
    const float halfW = GetWidth() * 0.5f;
    const float halfH = GetHeight() * 0.5f;

    const float x0 = -halfW;
    const float y0 = -halfH;

    const float x1 = halfW;
    const float y1 = halfH;

    const float c = cosf(mRot);
    const float s = sinf(mRot);

    const float rx0 = x0 * c - y0 * s;
    const float ry0 = y0 * c + x0 * s;

    const float rx1 = x1 * c - y1 * s;
    const float ry1 = y1 * c + x1 * s;

    mRotW = fabsf(roundf(rx1 - rx0));
    mRotH = fabsf(roundf(ry1 - ry0));
}

} // namespace graphic
} // namespace lib
