#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class ButtonMinimap;
class CameraMapController;
class IsoMap;
class MiniMap;
class PanelResources;
class Player;

// UI
class GameHUD : public sgl::sgui::Widget
{
public:
    GameHUD(Player * player, CameraMapController * camController, IsoMap * isoMap);

    MiniMap * GetMinimap() const;

private:
    PanelResources * mPanelRes = nullptr;
    ButtonMinimap * mButtonMinimap = nullptr;
    MiniMap * mMiniMap = nullptr;
};

inline MiniMap * GameHUD::GetMinimap() const { return mMiniMap; }

} // namespace game
