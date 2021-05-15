#pragma once

#include "Renderable.h"

namespace lib
{
namespace graphic
{

class Texture;

class TexturedRenderable : public Renderable
{
public:
    TexturedRenderable() = default;
    TexturedRenderable(Texture * tex);

    double GetRotation() const;
    void SetRotation(double degs);

    int GetRotatedWidth() const;
    int GetRotatedHeight() const;

    void SetTexture(Texture * tex);

    void Render() override;

public:
    static const int FLIP_NO;
    static const int FLIP_HORIZ;
    static const int FLIP_VERT;

protected:
    void DeleteTexture();

private:
    void UpdateRotatedSize();

private:
    Texture * mTex = nullptr;

    double mRot = 0;
    int mFlip = FLIP_NO;

    int mRotW = 0;
    int mRotH = 0;
};

inline double TexturedRenderable::GetRotation() const { return mRot; }
inline void TexturedRenderable::SetRotation(double degs)
{
    mRot = degs;

    UpdateRotatedSize();
}

inline int TexturedRenderable::GetRotatedWidth() const { return mRotW; }
inline int TexturedRenderable::GetRotatedHeight() const { return mRotH; }

} // namespace graphic
} // namespace lib
