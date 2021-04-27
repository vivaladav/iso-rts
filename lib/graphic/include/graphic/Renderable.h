#pragma once

struct SDL_Rect;

namespace lib
{
namespace graphic
{

class Camera;

class Renderable
{
public:
    Renderable();
    virtual ~Renderable();

    void SetPosition(int x, int y);
    int GetX() const;
    void SetX(int x);
    int GetY() const;
    void SetY(int y);

    void Scale(float s);
    void ScaleW(float s);
    void ScaleH(float s);

    void RestoreOriginalSize();

    int GetWidth() const;
    void SetWidth(int w);
    int GetHeight() const;
    void SetHeight(int h);

    void SetAlpha(unsigned char a);
    // get color as #RRGGBBAA
    unsigned int GetColor();
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    // color as #RRGGBBAA
    void SetColor(unsigned int color);

    void SetCamera(Camera * cam);

    virtual void Render() = 0;

protected:
    SDL_Rect * mRect = nullptr;

    Camera * mCamera = nullptr;

    int mOrigW = 0;
    int mOrigH = 0;

    unsigned char mR = 255;
    unsigned char mG = 255;
    unsigned char mB = 255;
    unsigned char mA = 255;
};

inline void Renderable::SetAlpha(unsigned char a) { mA = a; }

inline unsigned int Renderable::GetColor()
{
    return mR << 24 | mG << 16 | mB << 8 | mA;
}

inline void Renderable::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mR = r;
    mG = g;
    mB = b;
    mA = a;
}

inline void Renderable::SetCamera(Camera * cam) { mCamera = cam; }

} // namespace graphic
} // namespace lib
