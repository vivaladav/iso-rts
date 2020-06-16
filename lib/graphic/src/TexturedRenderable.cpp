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

TexturedRenderable::TexturedRenderable()
{
    mRect = new SDL_Rect{0, 0, 0, 0};
}

TexturedRenderable::~TexturedRenderable()
{
    delete mRect;
}

void TexturedRenderable::SetPosition(int x, int y)
{
    mRect->x = x;
    mRect->y = y;
}

void TexturedRenderable::SetX(int x) { mRect->x = x; }

void TexturedRenderable::SetY(int y) { mRect->y = y; }

void TexturedRenderable::Scale(float s)
{
    mRect->w *= s;
    mRect->h *= s;
}

void TexturedRenderable::ScaleW(float s) { mRect->w *= s; }

void TexturedRenderable::ScaleH(float s) { mRect->h *= s; }

int TexturedRenderable::GetWidth() const { return mRect->w; }
int TexturedRenderable::GetHeight() const { return mRect->h; }

void TexturedRenderable::SetFlip(int flip)
{
    if(flip > SDL_FLIP_NONE && flip <= SDL_FLIP_VERTICAL)
        mFlip = flip;
    else
        mFlip = SDL_FLIP_NONE;
}

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
}

void TexturedRenderable::DeleteTexture()
{
    delete mTex;
    mTex = nullptr;
}

} // namespace graphic
} // namespace lib
