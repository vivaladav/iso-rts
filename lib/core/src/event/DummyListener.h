#pragma once

#include "core/event/EventListener.h"

namespace lib
{
namespace core
{

class Application;

class DummyListener : public EventListener
{
public:
    DummyListener(Application * app);

private:
    // -- application events --
    void OnApplicationQuit() override;

private:
    Application * mApp = nullptr;
};

} // namespace core
} // namespace lib
