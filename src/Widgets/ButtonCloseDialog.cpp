#include "Widgets/ButtonCloseDialog.h"

#include <graphic/Image.h>

namespace game
{

ButtonCloseDialog::ButtonCloseDialog(lib::graphic::Texture * texIcon, int w, int h,
                                     lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
    , mIcon(new lib::graphic::Image(texIcon))
{
    SetSize(w, h);

    RegisterRenderable(mIcon);
}

void ButtonCloseDialog::HandlePositionChanged()
{
    const int x = GetScreenX();
    const int y = GetScreenY();

    const int iconX = x + (GetWidth() - mIcon->GetWidth()) * 0.5f;
    const int iconY = y + (GetHeight() - mIcon->GetHeight()) * 0.5f;

    mIcon->SetPosition(iconX, iconY);
}

void ButtonCloseDialog::OnRender()
{
    mIcon->Render();
}

} // namespace game
