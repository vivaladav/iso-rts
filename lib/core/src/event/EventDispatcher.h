#pragma once

#include <vector>

namespace lib
{
namespace core
{

class ApplicationEventListener;
class KeyboardEventListener;
class MouseEventListener;

class EventDispatcher
{
public:
    void AddApplicationListener(ApplicationEventListener * el);
    void AddKeyboardListener(KeyboardEventListener * el);
    void AddMouseListener(MouseEventListener * el);

    void RemoveApplicationListener(ApplicationEventListener * el);
    void RemoveKeyboardListener(KeyboardEventListener * el);
    void RemoveMouseListener(MouseEventListener * el);

    void Update();

private:
    std::vector<ApplicationEventListener *> mApplicationListeners;
    std::vector<KeyboardEventListener *> mKeyboardListeners;
    std::vector<MouseEventListener *> mMouseListeners;
};

} // namespace core
} // namespace lib

