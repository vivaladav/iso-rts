#pragma once

namespace lib
{
    namespace graphic
    {
        class Image;
        class Texture;
    }
}

namespace game
{

/// An object of an IsoLayer.
class IsoObject
{
public:
    IsoObject(int rows, int cols);
    ~IsoObject();

    int GetRows() const;
    int GetCols() const;

    void SetPosition(int x, int y);
    int GetX() const;
    void SetX(int x);
    int GetY() const;
    void SetY(int y);

    int GetWidth() const;
    int GetHeight() const;

    void SetTexture(lib::graphic::Texture * tex);

    void Render();

private:
    lib::graphic::Image * mImg = nullptr;

    int mRows = 0;
    int mCols = 0;
};

inline int IsoObject::GetRows() const { return mRows; }
inline int IsoObject::GetCols() const { return mCols; }

} // namespace game
