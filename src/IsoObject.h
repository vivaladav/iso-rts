#pragma once

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Texture;
    }
}

namespace game
{

class IsoLayer;

/// An object of an IsoLayer.
class IsoObject
{
public:
    IsoObject(int rows, int cols);
    virtual ~IsoObject();

    IsoLayer * GetLayer() const;
    void SetLayer(IsoLayer * layer);

    int GetRow() const;
    void SetRow(int r);
    int GetCol() const;
    void SetCol(int c);

    int GetRows() const;
    int GetCols() const;

    void SetPosition(int x, int y);
    int GetX() const;
    void SetX(int x);
    int GetY() const;
    void SetY(int y);

    int GetWidth() const;
    int GetHeight() const;

    void SetTexture(sgl::graphic::Texture * tex);
    void SetAlpha(unsigned char alpha);
    void SetColor(unsigned int color);

    bool IsVisible() const;
    void SetVisible(bool vis);

    virtual void Render();

private:
    virtual void OnPositionChanged();

private:
    sgl::graphic::Image * mImg = nullptr;

    IsoLayer * mLayer = nullptr;

    int mRows = 0;
    int mCols = 0;

    int mRow = 0;
    int mCol = 0;

    bool mVisible = true;
};

inline IsoLayer * IsoObject::GetLayer() const { return mLayer; }
inline void IsoObject::SetLayer(IsoLayer * layer) { mLayer = layer; }

inline int IsoObject::GetRow() const { return mRow; }
inline void IsoObject::SetRow(int r) { mRow = r; }
inline int IsoObject::GetCol() const { return mCol; }
inline void IsoObject::SetCol(int c) { mCol = c; }

inline int IsoObject::GetRows() const { return mRows; }
inline int IsoObject::GetCols() const { return mCols; }

inline bool IsoObject::IsVisible() const { return mVisible; }
inline void IsoObject::SetVisible(bool vis) { mVisible = vis; }

} // namespace game
