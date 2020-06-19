#include "graphic/Renderable.h"

#include <SDL2/SDL.h>

namespace lib
{
namespace graphic
{

Renderable::Renderable()
{
    mRect = new SDL_Rect{0, 0, 0, 0};
}

Renderable::~Renderable()
{
    delete mRect;
}

void Renderable::SetPosition(int x, int y)
{
    mRect->x = x;
    mRect->y = y;
}

void Renderable::SetX(int x) { mRect->x = x; }

void Renderable::SetY(int y) { mRect->y = y; }

void Renderable::Scale(float s)
{
    mRect->w *= s;
    mRect->h *= s;
}

void Renderable::ScaleW(float s) { mRect->w *= s; }

void Renderable::ScaleH(float s) { mRect->h *= s; }

int Renderable::GetWidth() const { return mRect->w; }
int Renderable::GetHeight() const { return mRect->h; }

} // namespace graphic
} // namespace lib
