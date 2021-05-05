#include "Widgets/SlotDialogNewUnit.h"

#include "Widgets/GameUIData.h"

#include <graphic/Camera.h>
#include <graphic/Image.h>
#include <graphic/TextureManager.h>

namespace game
{

SlotDialogNewUnit::SlotDialogNewUnit(lib::sgui::Widget * parent)
    : lib::sgui::Widget(parent)
    , mBg(new lib::graphic::Image)
{
    using namespace lib::graphic;

    // BACKGROUND
    auto tm = TextureManager::Instance();

    mBg->SetTexture(tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_DIALOG_BG_SLOT));
    RegisterRenderable(mBg);

    const int w = mBg->GetWidth();
    const int h = mBg->GetHeight();

    SetSize(w, h);
}

SlotDialogNewUnit::~SlotDialogNewUnit()
{
    delete mBg;
}

void SlotDialogNewUnit::HandlePositionChanged()
{
    const int x = GetScreenX();
    const int y = GetScreenY();

    mBg->SetPosition(x, y);
}

void SlotDialogNewUnit::OnRender()
{
    mBg->Render();
}

} // namespace game
