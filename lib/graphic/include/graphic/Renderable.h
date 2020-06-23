#pragma once

struct SDL_Rect;

namespace lib
{
namespace graphic
{

class Renderable
{
public:
    Renderable();
    virtual ~Renderable();

    void SetPosition(int x, int y);
    void SetX(int x);
    void SetY(int y);

    void Scale(float s);
    void ScaleW(float s);
    void ScaleH(float s);

    int GetWidth() const;
    int GetHeight() const;

    void SetAlpha(unsigned char a);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    // color as #RRGGBBAA
    void SetColor(unsigned int color);

    virtual void Render() = 0;

protected:
    SDL_Rect * mRect = nullptr;

    unsigned char mR = 255;
    unsigned char mG = 255;
    unsigned char mB = 255;
    unsigned char mA = 255;
};

inline void Renderable::SetAlpha(unsigned char a) { mA = a; }

inline void Renderable::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mR = r;
    mG = g;
    mB = b;
    mA = a;
}

} // namespace graphic
} // namespace lib