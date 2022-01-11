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
    , mFontLabel(Stage::Instance()->GetDefaultFont())
    , mBg(new graphic::DummyRenderable)
    , mLabel(new graphic::DummyRenderable)
    , mCurrBg(mBg)
    , mCurrLabel(mLabel)
{
    RegisterRenderable(mBg);
    RegisterRenderable(mLabel);
}

PushButton::~PushButton()
{
    delete mBg;
    delete mLabel;
}

void PushButton::SetBackground(const char * file)
{
    assert(file);

    UnregisterRenderable(mBg);
    delete mBg;

    mBg = new graphic::Image(file); // TODO check if Image is valid after creation and fallback on dummy
    RegisterRenderable(mBg);

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

    UnregisterRenderable(mLabel);
    delete mLabel;

    if(mText.empty())
        mLabel = new graphic::DummyRenderable;
    else
        mLabel = new graphic::Text(text, mFontLabel);

    RegisterRenderable(mLabel);

    mLabel->SetColor(col);

    SetCurrLabel(mLabel);
}

void PushButton::SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    a = MixAlphaAndAlpha(a);

    mCurrLabel->SetColor(r, g, b, a);
}

void PushButton::SetLabelColor(unsigned int color)
{
    color = MixColorAndAlpha(color);

    mCurrLabel->SetColor(color);
}

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

        UnregisterRenderable(mLabel);
        delete mLabel;

        mLabel = new graphic::Text(mText.c_str(), mFontLabel);
        RegisterRenderable(mLabel);

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

        for(auto & f : mOnToggle)
            f(mChecked);
    }
}

void PushButton::Click()
{
    // checkable button
    if(mCheckable)
    {
        mChecked = !mChecked;

        HandleCheckedChanged(mChecked);

        for(auto & f : mOnToggle)
            f(mChecked);
    }
    // standard button
    else
        for(auto & f : mOnClick)
            f();
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

void PushButton::HandleStateEnabled()
{
    if(IsChecked())
        SetState(CHECKED);
    else
        SetState(NORMAL);
}
void PushButton::HandleStateDisabled()
{
     SetState(DISABLED);
}

void PushButton::HandleMouseButtonDown(lib::core::MouseButtonEvent & event)
{
    if(!IsEnabled())
        return ;

    HandleButtonDown();

    event.SetConsumed();
}

void PushButton::HandleMouseButtonUp(core::MouseButtonEvent & event)
{
    if(!IsEnabled())
        return ;

    HandleButtonUp();

    event.SetConsumed();
}

void PushButton::HandleMouseOver()
{
    if(!IsEnabled() || IsChecked())
        return ;

    SetState(MOUSE_OVER);
}

void PushButton::HandleMouseOut()
{
    if(!IsEnabled() || IsChecked())
        return ;

    SetState(NORMAL);
}

void PushButton::HandleCheckedChanged(bool checked)
{
    SetState(checked ? CHECKED : NORMAL);
}

void PushButton::OnStateChanged(VisualState state) { }

void PushButton::HandleButtonDown()
{
    if(IsCheckable())
        SetState(CHECKED);
    else
        SetState(PUSHED);
}

void PushButton::HandleButtonUp()
{
    // set default state then handle click
    SetState(NORMAL);

    Click();
}

void PushButton::SetState(VisualState state)
{
    if(mState == state)
        return ;

    mState = state;

    OnStateChanged(state);
}

void PushButton::PositionLabel()
{
    const int x = GetScreenX() + (GetWidth() - mCurrLabel->GetWidth()) * 0.5f;
    const int y = GetScreenY() + (GetHeight() - mCurrLabel->GetHeight()) * 0.5f;

    mCurrLabel->SetPosition(x, y);
}

} // namespace sgui
} // namespace lib
