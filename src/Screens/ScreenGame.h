#pragma once

#include "Screen.h"

#include <core/event/EventListener.h>

namespace game
{

class IsoMap;

class ScreenGame : public Screen, public lib::core::EventListener
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update() override;
    void Render() override;

    void OnMouseButtonUp(const lib::core::MouseButtonEvent & event) override;

private:
    void OnActive() override;
    void OnInactive() override;

private:
    IsoMap * mMap = nullptr;
};

} // namespace game
