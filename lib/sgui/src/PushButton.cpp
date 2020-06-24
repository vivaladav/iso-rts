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
    , mBg(new graphic::DummyRenderable)
    , mLabel(new graphic::DummyRenderable)
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
    mBg->SetPosition(GetScreenX(), GetScreenY());

    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();
    SetSize(w, h);
}

void PushButton::SetLabel(const char * text)
{
    delete mLabel;

    std::string t(text);

    if(t.empty())
        mLabel = new graphic::DummyRenderable;
    else
    {
        // TODO proper font handling
        graphic::Font * font = Stage::Instance()->GetDefaultFont();

        mLabel = new graphic::Text(text, font);
    }

    PositionLabel();
}

void PushButton::SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mLabel->SetColor(r, g, b, a);
}

void PushButton::SetLabelColor(unsigned int color) { mLabel->SetColor(color); }

void PushButton::OnPositionChanged()
{
    mBg->SetPosition(GetScreenX(), GetScreenY());

    PositionLabel();
}

void PushButton::PositionLabel()
{
    const int x = GetScreenX() + (GetWidth() - mLabel->GetWidth()) * 0.5f;
    const int y = GetScreenY() + (GetHeight() - mLabel->GetHeight()) * 0.5f;

    mLabel->SetPosition(x, y);
}

void PushButton::HandleMouseButtonUp(core::MouseButtonEvent & event)
{
    event.SetConsumed();

    mOnClick();
}

void PushButton::OnRender()
{
    mBg->Render();

    mLabel->Render();
}

} // namespace sgui
} // namespace lib
