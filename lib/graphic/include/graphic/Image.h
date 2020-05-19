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

    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    void Scale(float s);
    void ScaleW(float s);
    void ScaleH(float s);

    void Render();

private:
    SDL_Texture * mTex = nullptr;

    SDL_Rect * mRect = nullptr;
};

} // namespace graphic
} // namespace lib
