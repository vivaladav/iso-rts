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
    void SetRotation(double degs);

    void SetTexture(Texture * tex);

    void Render() override;

public:
    static const int FLIP_NO;
    static const int FLIP_HORIZ;
    static const int FLIP_VERT;

protected:
    void DeleteTexture();

private:
    Texture * mTex = nullptr;

    double mRot = 0;
    int mFlip = FLIP_NO;
};

inline void TexturedRenderable::SetRotation(double degs) { mRot = degs; }

} // namespace graphic
} // namespace lib
