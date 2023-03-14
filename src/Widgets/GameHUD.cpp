#include "GameHUD.h"

#include "Widgets/ButtonMinimap.h"
#include "Widgets/MiniMap.h"
#include "Widgets/PanelResources.h"

#include <sgl/graphic/Renderer.h>

namespace game
{

GameHUD::GameHUD(Player * player, CameraMapController * camController, IsoMap * isoMap)
{
    using namespace sgl;

    const int rendW = graphic::Renderer::Instance()->GetWidth();
    const int rendH = graphic::Renderer::Instance()->GetHeight();

    SetSize(rendW, rendH);

    // TOP RESOURCE BAR
    mPanelRes = new PanelResources(player, this);
    mPanelRes->SetX((rendW - mPanelRes->GetWidth()) / 2);

    // MINIMAP
    mButtonMinimap = new ButtonMinimap(this);
    mButtonMinimap->SetX(rendW - mButtonMinimap->GetWidth());

    mButtonMinimap->AddOnClickFunction([this]
    {
        mButtonMinimap->SetVisible(false);
        mMiniMap->SetVisible(true);
    });

    mMiniMap = new MiniMap(camController, isoMap, this);
    mMiniMap->SetVisible(false);
    mMiniMap->SetX(rendW - mMiniMap->GetWidth());

    mMiniMap->AddFunctionOnClose([this]
    {
        mButtonMinimap->SetVisible(true);
        mMiniMap->SetVisible(false);
    });
}

} // namespace game
