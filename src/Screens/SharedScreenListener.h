#pragma once

#include <sgl/core/event/ApplicationEventListener.h>
#include <sgl/core/event/KeyboardEventListener.h>

namespace game
{

class Screen;

class SharedScreenListener : public sgl::core::ApplicationEventListener, public sgl::core::KeyboardEventListener
{
public:
    SharedScreenListener(Screen * parent);

    void OnApplicationQuit(sgl::core::ApplicationEvent & event) override;
    void OnKeyUp(sgl::core::KeyboardEvent & event) override;

private:
    Screen * mScreen = nullptr;
};

inline SharedScreenListener::SharedScreenListener(Screen * parent) : mScreen(parent)
{
}

} // namespace game
