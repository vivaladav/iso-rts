#pragma once

#include <sgl/sgui/Widget.h>

namespace sgl
{
    namespace sgui { class ButtonsGroup; }
}

namespace game
{

class ButtonMinimap;
class CameraMapController;
class IsoMap;
class MiniMap;
class PanelResources;
class Player;
class ScreenGame;

// UI
class GameHUD : public sgl::sgui::Widget
{
public:
    GameHUD(Player * player, CameraMapController * camController,
            IsoMap * isoMap, ScreenGame * screen);

    MiniMap * GetMinimap() const;
    sgl::sgui::ButtonsGroup * GetButtonsGroupUnitSel() const;

private:
    PanelResources * mPanelRes = nullptr;
    ButtonMinimap * mButtonMinimap = nullptr;
    MiniMap * mMiniMap = nullptr;
    sgl::sgui::ButtonsGroup * mGroupUnitSel = nullptr;
};

inline MiniMap * GameHUD::GetMinimap() const { return mMiniMap; }

inline sgl::sgui::ButtonsGroup * GameHUD::GetButtonsGroupUnitSel() const { return mGroupUnitSel; }

} // namespace game
