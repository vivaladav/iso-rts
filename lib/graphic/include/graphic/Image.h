#pragma once

struct SDL_Rect;

namespace lib
{
namespace graphic
{

class Texture;

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

    void SetRotation(double degs);

    void SetFlip(int flip);

    void Render();

public:
    static const int FLIP_NO;
    static const int FLIP_HORIZ;
    static const int FLIP_VERT;

private:
    Texture * mTex = nullptr;

    SDL_Rect * mRect = nullptr;
    double mRot = 0;
    int mFlip;
};

inline void Image::SetRotation(double degs) { mRot = degs; }

} // namespace graphic
} // namespace lib
