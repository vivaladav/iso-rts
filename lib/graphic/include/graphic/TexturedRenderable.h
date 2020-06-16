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

    void SetAlpha(unsigned char a);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

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

    unsigned char mR = 255;
    unsigned char mG = 255;
    unsigned char mB = 255;
    unsigned char mA = 255;
};

inline void TexturedRenderable::SetRotation(double degs) { mRot = degs; }

inline void TexturedRenderable::SetAlpha(unsigned char a) { mA = a; }

inline void TexturedRenderable::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mR = r;
    mG = g;
    mB = b;
    mA = a;
}


} // namespace graphic
} // namespace lib
