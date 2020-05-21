#pragma once

#include <core/event/EventListener.h>
#include <utilities/State.h>

class Game;

class Screen : public lib::core::EventListener, public lib::utilities::State
{
public:
    Screen(unsigned int screenId, Game * app);
    virtual ~Screen();

    virtual void Update() = 0;

    void OnActive();

protected:
    Game * mApp;
};

inline Screen::Screen(unsigned int screenId, Game * app)
    : lib::utilities::State(screenId)
    , mApp(app)
{
}

inline Screen::~Screen() { }
