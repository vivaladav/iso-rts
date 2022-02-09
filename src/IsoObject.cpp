#include "IsoObject.h"

#include "IsoLayer.h"

#include <sgl/graphic/Image.h>

namespace game
{

IsoObject::IsoObject(int rows, int cols)
    : mImg(new sgl::graphic::Image)
    , mRows(rows)
    , mCols(cols)
{
}

IsoObject::~IsoObject()
{
    delete mImg;
}

void IsoObject::SetPosition(int x, int y)
{
    mImg->SetPosition(x, y);
    OnPositionChanged();
}

int IsoObject::GetX() const { return mImg->GetX(); }

void IsoObject::SetX(int x)
{
    mImg->SetX(x);
    OnPositionChanged();
}

int IsoObject::GetY() const { return mImg->GetY(); }

void IsoObject::SetY(int y)
{
    mImg->SetY(y);
    OnPositionChanged();
}

int IsoObject::GetWidth() const { return mImg->GetWidth(); }
int IsoObject::GetHeight() const { return mImg->GetHeight(); }

void IsoObject::SetTexture(sgl::graphic::Texture * tex)
{
    mImg->SetTexture(tex);

    if(mLayer)
        mLayer->RepositionObject(this);
}

void IsoObject::SetAlpha(unsigned char alpha) { mImg->SetAlpha(alpha); }

void IsoObject::SetColor(unsigned int color) { mImg->SetColor(color); }

void IsoObject::Render() { mImg->Render(); }

void IsoObject::OnPositionChanged() {}

} // namespace game
