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
class DialogExploreTemple;
class DialogExploreTempleOutcome;
class DialogNewElement;
class GameMapProgressBar;
class MiniMap;
class PanelObjectActions;
class PanelResources;
class Player;
class ScreenGame;
class Temple;
struct Cell2D;

enum PlayerFaction : unsigned int;

// UI
class GameHUD : public sgl::sgui::Widget
{
public:
    GameHUD(ScreenGame * screen);
    ~GameHUD();

    void SetMiniMapEnabled(bool val);
    MiniMap * GetMinimap() const;
    sgl::sgui::ButtonsGroup * GetButtonsGroupUnitSel() const;

    PanelObjectActions * GetPanelObjectActions() const;
    void HidePanelObjActions();

    void ShowDialogEndMission(bool won);
    void ShowDialogExit();
    void ShowDialogExploreTemple(Player * player, Temple * temple);
    void HideDialogExploreTemple();
    void ShowDialogNewElement(unsigned int type);
    void HideDialogNewElement();

    void ShowMissionCountdown(int secs);
    void HideMissionCountdown();

    GameMapProgressBar * CreateProgressBarInCell(const Cell2D & cell, float time, PlayerFaction faction);

private:
    void ShowDialogExploreTempleOutcome(Player * player, Temple * temple);
    void HideDialogExploreTempleOutcome();

    GameMapProgressBar * CreateProgressBar(float time, PlayerFaction faction);

    void CenterWidget(sgl::sgui::Widget * w);

private:
    PanelResources * mPanelRes = nullptr;
    ButtonMinimap * mButtonMinimap = nullptr;
    MiniMap * mMiniMap = nullptr;
    PanelObjectActions * mPanelObjActions = nullptr;
    sgl::sgui::ButtonsGroup * mGroupUnitSel = nullptr;
    CountdownLabel * mCountdownLabel = nullptr;

    DialogExit * mDialogExit = nullptr;
    DialogExploreTemple * mDialogExploreTemple = nullptr;
    DialogExploreTempleOutcome * mDialogExploreTempleOutcome = nullptr;
    DialogNewElement * mDialogNewElement = nullptr;

    ScreenGame * mScreen = nullptr;
};

inline MiniMap * GameHUD::GetMinimap() const { return mMiniMap; }

inline sgl::sgui::ButtonsGroup * GameHUD::GetButtonsGroupUnitSel() const { return mGroupUnitSel; }

inline PanelObjectActions * GameHUD::GetPanelObjectActions() const { return mPanelObjActions; }

} // namespace game
