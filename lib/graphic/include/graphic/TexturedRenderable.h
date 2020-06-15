#pragma once

struct SDL_Rect;

namespace lib
{
namespace graphic
{

class Texture;

class TexturedRenderable
{
public:
    TexturedRenderable();
    ~TexturedRenderable();

    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    void Scale(float s);
    void ScaleW(float s);
    void ScaleH(float s);

    int GetWidth() const;
    int GetHeight() const;

    void SetRotation(double degs);

    void SetFlip(int flip);

    void Render();

public:
    static const int FLIP_NO;
    static const int FLIP_HORIZ;
    static const int FLIP_VERT;

protected:
    void SetTexture(Texture * tex);

    void DeleteTexture();

private:
    Texture * mTex = nullptr;

    SDL_Rect * mRect = nullptr;
    double mRot = 0;
    int mFlip = FLIP_NO;
};

inline void TexturedRenderable::SetRotation(double degs) { mRot = degs; }

} // namespace graphic
} // namespace lib
