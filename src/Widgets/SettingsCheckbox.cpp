#include "Widgets/SettingsCheckbox.h"

#include "Widgets/GameUIData.h"

#include <sgl/graphic/Image.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/media/AudioManager.h>
#include <sgl/media/AudioPlayer.h>

namespace game
{

SettingsCheckbox::SettingsCheckbox(sgl::sgui::Widget * parent)
    : sgl::sgui::AbstractButton(parent)
{
    using namespace sgl;

    SetCheckable(true);

    // BAR
    mBg = new graphic::Image;
    RegisterRenderable(mBg);

    // UPDATE CONTENT
    UpdateGraphics(NORMAL);
    UpdatePositions();
}

void SettingsCheckbox::HandleButtonDown()
    {
        sgl::sgui::AbstractButton::HandleButtonDown();

        auto player = sgl::media::AudioManager::Instance()->GetPlayer();
        player->PlaySound("UI/checkbox-01.ogg");
    }

void SettingsCheckbox::OnStateChanged(sgl::sgui::AbstractButton::VisualState state)
{
    UpdateGraphics(state);
}

void SettingsCheckbox::UpdateGraphics(sgl::sgui::AbstractButton::VisualState state)
{
    using namespace sgl;

    auto tm = graphic::TextureManager::Instance();

    const unsigned int texIDs[] =
        {
            ID_DLG_SETT_CHB_NORMAL,
            ID_DLG_SETT_CHB_DISABLED,
            ID_DLG_SETT_CHB_OVER,
            ID_DLG_SETT_CHB_PUSHED,
            ID_DLG_SETT_CHB_CHECKED,
        };

    graphic::Texture * tex = nullptr;

    if(IsEnabled())
        tex = tm->GetSprite(SpriteFileDialogSettings, texIDs[state]);
    else
    {
        const unsigned int texID = IsChecked() ?
                                   ID_DLG_SETT_CHB_CHECKED_DISABLED : ID_DLG_SETT_CHB_DISABLED;
        tex = tm->GetSprite(SpriteFileDialogSettings, texID);
    }

    mBg->SetTexture(tex);

    SetSize(mBg->GetWidth(), mBg->GetHeight());
}

void SettingsCheckbox::HandlePositionChanged()
{
    UpdatePositions();
}

void SettingsCheckbox::UpdatePositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();
    mBg->SetPosition(x0, y0);
}

} // namespace game