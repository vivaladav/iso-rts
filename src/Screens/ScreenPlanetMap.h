#pragma once

#include "Screen.h"

#ifdef DEMO_MODE
#include <sgl/utilities/StringsChangeListener.h>
#endif

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class Label; }
}

namespace game
{

class DialogExit;
class PanelResources;
class PanelPlanetActionConquer;
class PanelPlanetActionConquerAI;
class PanelPlanetActionExplore;
class PanelPlanetActions;
class PanelPlanetInfo;
class PanelPlanetResources;
class PlanetMap;

#ifdef DEMO_MODE
class GameSimpleTooltip;

class ScreenPlanetMap : public Screen, public sgl::utilities::StringsChangeListener
#else
class ScreenPlanetMap : public Screen
#endif
{
public:
    ScreenPlanetMap(Game * game);
    ~ScreenPlanetMap();

    bool Save(sgl::utilities::BinaryFile & bf) const override;

    void OnKeyUp(sgl::core::KeyboardEvent & event) override;

    void Update(float delta) override;
    void Render() override;

    void SetPause(bool paused);

private:
    void SetPlanetName(const char * name);
    void SetDate(const char * date);

    void ShowInfo(int territory);

    bool TryToConquerTerritory(int index);

    void UpdatePlanetButtons();

    void ShowDialogExit();

#ifdef DEMO_MODE
    void OnStringsChanged() override;
#endif

private:
    friend class TutorialPlanetMap;

    sgl::graphic::Image * mBg = nullptr;

    PanelResources * mPanelPlayerRes = nullptr;
    PanelPlanetInfo * mPanelInfo = nullptr;
    PanelPlanetResources * mPanelResources = nullptr;
    PanelPlanetActions * mPanelActions = nullptr;
    PanelPlanetActionExplore * mPanelExplore = nullptr;
    PanelPlanetActionConquer * mPanelConquer = nullptr;
    PanelPlanetActionConquerAI * mPanelConquerAI = nullptr;

    sgl::sgui::Label * mLabelName = nullptr;
    sgl::sgui::Label * mLabelDate = nullptr;

    PlanetMap * mPlanetMap = nullptr;

    DialogExit * mDialogExit = nullptr;

#ifdef DEMO_MODE
    GameSimpleTooltip * mTooltipDemo = nullptr;
#endif

    bool mPaused = false;
};

} // namespace game
