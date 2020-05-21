#pragma once

#include <core/Application.h>

namespace lib
{
    namespace core
    {
        class EventListener;
    }

    namespace graphic
    {
        class Renderer;
        class Window;
    }

    namespace utilities { class StateManager; }
}

class Game : public lib::core::Application
{
public:
    enum Screens : int
    {
        TEST = 1
    };

public:
    Game(int argc, char * argv[]);
    ~Game();

private:
    void Update() override;

private:
    lib::graphic::Renderer * mRenderer = nullptr;
    lib::graphic::Window * mWin = nullptr;

    lib::utilities::StateManager * mStateMan = nullptr;
};
