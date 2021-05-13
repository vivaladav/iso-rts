#include "graphic/TexturedRenderable.h"

#include "graphic/Camera.h"
#include "graphic/Renderer.h"
#include "graphic/Texture.h"
#include "graphic/TextureData.h"

#include <SDL2/SDL.h>

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
}

void TexturedRenderable::DeleteTexture()
{
    delete mTex;
    mTex = nullptr;
}

} // namespace graphic
} // namespace lib
