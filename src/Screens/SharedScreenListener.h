#pragma once

#include <core/event/EventListener.h>

namespace game
{

class Screen;

class SharedScreenListener : public lib::core::EventListener
{
public:
    SharedScreenListener(Screen * parent);

    void OnApplicationQuit() override;
    void OnKeyUp(const lib::core::KeyboardEvent & event) override;

private:
    Screen * mScreen = nullptr;
};

inline SharedScreenListener::SharedScreenListener(Screen * parent) : mScreen(parent)
{
}

} // namespace game
