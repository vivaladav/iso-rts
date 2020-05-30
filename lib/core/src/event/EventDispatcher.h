#pragma once

#include <vector>

namespace lib
{
namespace core
{

class EventListener;

class EventDispatcher
{
public:
    void AddListener(EventListener * listener);

    void Update();

private:
    std::vector<EventListener *> mListeners;
};

inline void EventDispatcher::AddListener(lib::core::EventListener * el) { mListeners.emplace_back(el); }

} // namespace core
} // namespace lib

