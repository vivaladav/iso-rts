#pragma once

#include <sgl/core/event/ApplicationEventListener.h>
#include <sgl/core/event/KeyboardEventListener.h>

namespace game
{

class Screen;

class SharedScreenListener : public lib::core::ApplicationEventListener, public lib::core::KeyboardEventListener
{
public:
    SharedScreenListener(Screen * parent);

    void OnApplicationQuit(lib::core::ApplicationEvent & event) override;
    void OnKeyUp(lib::core::KeyboardEvent & event) override;

private:
    Screen * mScreen = nullptr;
};

inline SharedScreenListener::SharedScreenListener(Screen * parent) : mScreen(parent)
{
}

} // namespace game
