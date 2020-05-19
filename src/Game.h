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
        class Image;
        class Renderer;
        class Window;
    }
}

class Game : public lib::core::Application
{
public:
    Game(int argc, char * argv[]);
    ~Game();

private:
    void Update() override;

private:
    lib::core::EventListener * mListener = nullptr;

    lib::graphic::Renderer * mRenderer = nullptr;
    lib::graphic::Window * mWin = nullptr;

    lib::graphic::Image * mImg1 = nullptr;
    lib::graphic::Image * mImg2 = nullptr;
    lib::graphic::Image * mImg3 = nullptr;
};
