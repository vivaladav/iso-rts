#include "Widgets/DialogNewUnit.h"

#include "Widgets/ButtonCloseDialog.h"
#include "Widgets/GameUIData.h"
#include "Widgets/SlotDialogNewUnit.h"

#include <graphic/Camera.h>
#include <graphic/TextureManager.h>
#include <sgui/Image.h>

namespace game
{

DialogNewUnit::DialogNewUnit()
{
    using namespace lib::sgui;

    // BACKGROUND
    auto tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex = nullptr;

    // left bg
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_DIALOG_BG_LEFT);
    mBgLeft = new Image(tex, this);

    // right bg
    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_DIALOG_BG_RIGHT);
    mBgRight = new Image(tex, this);

    // CLOSE BUTTON
    const int buttonSide = 20;

    tex = tm->GetSprite(SpriteFileNewUnitDialog, IND_NU_ICON_CLOSE);
    mButtonClose = new ButtonCloseDialog(tex, buttonSide, buttonSide, this);
}

DialogNewUnit::~DialogNewUnit()
{
    delete mBgLeft;
    delete mBgRight;
}

void DialogNewUnit::AddUnitSlot()
{
    auto slot = new SlotDialogNewUnit(this);

    mSlots.emplace_back(slot);

    RepositionElements();

    MoveChildToFront(mButtonClose);
}

void DialogNewUnit::SetFunctionOnClose(const std::function<void()> & f)
{
    mButtonClose->SetOnClickFunction(f);
}

void DialogNewUnit::RepositionElements()
{
    // left BG
    mBgLeft->SetPosition(0, 0);

    int elemX = mBgLeft->GetWidth();

    // slots
    for(auto slot : mSlots)
    {
        slot->SetPosition(elemX, 0);

        elemX += slot->GetWidth();
    }

    // right BG
    mBgRight->SetPosition(elemX, 0);

    // close button
    elemX = GetWidth() - mButtonClose->GetWidth();
    mButtonClose->SetPosition(elemX, 0);
}

} // namespace game
