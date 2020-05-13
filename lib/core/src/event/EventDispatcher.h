#pragma once

namespace lib
{
namespace core
{

class EventListener;

class EventDispatcher
{
public:
    EventDispatcher(EventListener * listener);

    void SetListener(EventListener * listener);

    void Update();

private:
    EventListener * mActiveListener = nullptr;
};

} // namespace core
} // namespace lib

