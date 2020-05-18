#pragma once

struct SDL_Rect;
struct SDL_Texture;

namespace lib
{
namespace graphic
{

class Image
{
public:
    Image(const char * file);

    ~Image();

    void Render();

private:
    SDL_Texture * mTex = nullptr;

    SDL_Rect * mRect = nullptr;
};

} // namespace graphic
} // namespace lib
