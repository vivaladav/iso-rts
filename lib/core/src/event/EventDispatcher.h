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
    void AddListener(EventListener * el);
    void RemoveListener(EventListener * el);

    void Update();

private:
    std::vector<EventListener *> mListeners;
};

} // namespace core
} // namespace lib

