#pragma once

#include "Screen.h"

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

class PlanetMap;

class ScreenPlanetMap : public Screen
{
public:
    ScreenPlanetMap(Game * game);
    ~ScreenPlanetMap();

    void Update(float update) override;
    void Render() override;

private:
    sgl::graphic::Image * mBg = nullptr;

    PlanetMap * mPlanet = nullptr;
};

} // namespace game
