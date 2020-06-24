#pragma once

#include <core/event/ApplicationEventListener.h>
#include <core/event/KeyboardEventListener.h>

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
