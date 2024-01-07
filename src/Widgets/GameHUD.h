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
class CountdownLabel;
class DialogExit;
class DialogNewElement;
class Game;
class GameMap;
class IsoMap;
class MiniMap;
class PanelObjectActions;
class PanelResources;
class Player;
class ScreenGame;

// UI
class GameHUD : public sgl::sgui::Widget
{
public:
    GameHUD(Player * player, CameraMapController * camController,
            IsoMap * isoMap, ScreenGame * screen, GameMap * gameMap);

    void SetMiniMapEnabled(bool val);
    MiniMap * GetMinimap() const;
    sgl::sgui::ButtonsGroup * GetButtonsGroupUnitSel() const;

    PanelObjectActions * GetPanelObjectActions() const;
    void HidePanelObjActions();

    void ShowDialogEndMission(bool won);
    void ShowDialogExit();
    DialogNewElement * GetDialogNewElement() const;
    DialogNewElement * ShowDialogNewElement(unsigned int type);
    void HideDialogNewElement();
    bool IsDialogNewElementVisible() const;

    void ShowMissionCountdown(int secs);
    void HideMissionCountdown();

private:
    PanelResources * mPanelRes = nullptr;
    ButtonMinimap * mButtonMinimap = nullptr;
    MiniMap * mMiniMap = nullptr;
    PanelObjectActions * mPanelObjActions = nullptr;
    sgl::sgui::ButtonsGroup * mGroupUnitSel = nullptr;
    CountdownLabel * mCountdownLabel = nullptr;

    DialogExit * mDialogExit = nullptr;
    DialogNewElement * mDialogNewElement = nullptr;

    Game * mGame = nullptr;
    GameMap * mGameMap = nullptr;
    ScreenGame * mScreen = nullptr;
};

inline MiniMap * GameHUD::GetMinimap() const { return mMiniMap; }

inline sgl::sgui::ButtonsGroup * GameHUD::GetButtonsGroupUnitSel() const { return mGroupUnitSel; }

inline PanelObjectActions * GameHUD::GetPanelObjectActions() const { return mPanelObjActions; }

inline DialogNewElement * GameHUD::GetDialogNewElement() const { return mDialogNewElement; }
inline bool GameHUD::IsDialogNewElementVisible() const { return mDialogNewElement != nullptr; }

} // namespace game
