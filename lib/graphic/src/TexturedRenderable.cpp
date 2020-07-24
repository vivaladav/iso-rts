#include "graphic/TexturedRenderable.h"

#include "graphic/Renderer.h"
#include "graphic/Texture.h"

#include <SDL2/SDL.h>

namespace lib
{
namespace graphic
{

const int TexturedRenderable::FLIP_NO    = SDL_FLIP_NONE;
const int TexturedRenderable::FLIP_HORIZ = SDL_FLIP_HORIZONTAL;
const int TexturedRenderable::FLIP_VERT  = SDL_FLIP_VERTICAL;

void TexturedRenderable::Render()
{
    mTex->SetColor(mR, mG, mB, mA);

    SDL_Renderer * r = Renderer::Instance()->mSysRenderer;
    const SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(mFlip);

    SDL_RenderCopyEx(r, mTex->mData, nullptr, mRect, mRot, nullptr, flip);
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
