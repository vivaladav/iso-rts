#pragma once

#include <sgui/Widget.h>

#include <functional>
#include <vector>

namespace lib
{
    namespace sgui { class ButtonsGroup; }
}

namespace game
{

class ButtonClose;
class ButtonSlot;
class Player;

enum UnitType : unsigned int;

class DialogNewElement : public lib::sgui::Widget
{
public:
    DialogNewElement(Player * player);
    ~DialogNewElement();

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

private:
    lib::sgui::ButtonsGroup * mSlots = nullptr;

    ButtonClose * mButtonClose = nullptr;
};

} // namespace game
