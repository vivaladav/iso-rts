#include "sgui/PushButton.h"

#include "core/event/MouseButtonEvent.h"
#include "graphic/DummyRenderable.h"
#include "graphic/Image.h"
#include "graphic/Text.h"
#include "sgui/Stage.h"

#include <cassert>

namespace lib
{
namespace sgui
{

PushButton::PushButton(Widget * parent)
    : Widget(parent)
    , mOnClick([]{})
    , mOnToggle([](bool){})
    , mFontLabel(Stage::Instance()->GetDefaultFont())
    , mBg(new graphic::DummyRenderable)
    , mLabel(new graphic::DummyRenderable)
    , mCurrBg(mBg)
    , mCurrLabel(mLabel)
{
}

PushButton::~PushButton()
{
    delete mBg;
    delete mLabel;
}

void PushButton::SetBackground(const char * file)
{
    assert(file);

    delete mBg;

    mBg = new graphic::Image(file); // TODO check if Image is valid after creation and fallback on dummy

    SetCurrBg(mBg);
}

void PushButton::SetLabel(const char * text)
{
    const std::string txt = text;

    // same text -> nothing to do
    if(txt == mText)
        return ;

    mText = text;

    const unsigned int col = mCurrLabel->GetColor();

    delete mLabel;

    if(mText.empty())
        mLabel = new graphic::DummyRenderable;
    else
        mLabel = new graphic::Text(text, mFontLabel);

    mLabel->SetColor(col);

    SetCurrLabel(mLabel);
}

void PushButton::SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mCurrLabel->SetColor(r, g, b, a);
}

void PushButton::SetLabelColor(unsigned int color) { mCurrLabel->SetColor(color); }

void PushButton::SetLabelFont(graphic::Font * font)
{
    if(!font)
        return ;

    // same font -> nothing to do
    if(font == mFontLabel)
        return ;

    mFontLabel = font;

    // when changing font, update the label if already set
    if(!mText.empty())
    {
        const unsigned int col = mCurrLabel->GetColor();

        delete mLabel;

        mLabel = new graphic::Text(mText.c_str(), mFontLabel);

        mLabel->SetColor(col);

        SetCurrLabel(mLabel);
    }
}

void PushButton::SetChecked(bool val)
{
    if(val != mChecked)
    {
        mChecked = val;
        HandleCheckedChanged(mChecked);

        mOnToggle(mChecked);
    }
}

void PushButton::SetCurrBg(graphic::Renderable * bg)
{
    mCurrBg = bg;

    mCurrBg->SetPosition(GetScreenX(), GetScreenY());

    const int w = mCurrBg->GetWidth();
    const int h = mCurrBg->GetHeight();
    SetSize(w, h);
}

void PushButton::SetCurrLabel(graphic::Renderable * label)
{
    mCurrLabel = label;

    PositionLabel();
}

void PushButton::HandlePositionChanged()
{
    mCurrBg->SetPosition(GetScreenX(), GetScreenY());

    PositionLabel();
}

void PushButton::PositionLabel()
{
    const int x = GetScreenX() + (GetWidth() - mCurrLabel->GetWidth()) * 0.5f;
    const int y = GetScreenY() + (GetHeight() - mCurrLabel->GetHeight()) * 0.5f;

    mCurrLabel->SetPosition(x, y);
}

void PushButton::HandleMouseButtonUp(core::MouseButtonEvent & event)
{
    event.SetConsumed();

    // checkable button
    if(mCheckable)
    {
        mChecked = !mChecked;

        HandleCheckedChanged(mChecked);

        mOnToggle(mChecked);
    }
    // standard button
    else
        mOnClick();
}

void PushButton::HandleCheckedChanged(bool) { }

void PushButton::OnRender()
{
    mCurrBg->Render();

    mCurrLabel->Render();
}

} // namespace sgui
} // namespace lib
