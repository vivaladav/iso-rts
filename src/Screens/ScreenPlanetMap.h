#pragma once

#include "Screen.h"

namespace sgl
{
    namespace graphic { class Image; }
    namespace sgui { class Label; }
}

namespace game
{

class PanelPlanetResources;
class PlanetMap;

class ScreenPlanetMap : public Screen
{
public:
    ScreenPlanetMap(Game * game);
    ~ScreenPlanetMap();

    void Update(float update) override;
    void Render() override;

private:
    void SetPlanetName(const char * name);
    void SetDate(const char * date);

private:
    sgl::graphic::Image * mBg = nullptr;

    PanelPlanetResources * mPanelResources = nullptr;

    sgl::sgui::Label * mLabelName = nullptr;
    sgl::sgui::Label * mLabelDate = nullptr;

    PlanetMap * mPlanet = nullptr;
};

} // namespace game
