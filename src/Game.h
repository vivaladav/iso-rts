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

namespace game
{

class Game : public lib::core::Application
{
public:
    Game(int argc, char * argv[]);
    ~Game();

    void SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    void Update() override;

private:
    lib::graphic::Renderer * mRenderer = nullptr;
    lib::graphic::Window * mWin = nullptr;

    lib::utilities::StateManager * mStateMan = nullptr;

    unsigned char mClearR = 0;
    unsigned char mClearG = 0;
    unsigned char mClearB = 0;
    unsigned char mClearA = 255;
};

inline void Game::SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mClearR = r;
    mClearG = g;
    mClearB = b;
    mClearA = a;
}

} // namespace game
