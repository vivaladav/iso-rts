#pragma once

#include <sgui/Widget.h>

#include <array>
#include <functional>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class GameObject;
class ObjectActionButton;

class PanelObjectActions : public lib::sgui::Widget
{
public:
    enum Button
    {
        BTN_BUILD_UNIT,
        BTN_MOVE,
        BTN_ATTACK,
        BTN_CONQUER_CELL,
        BTN_BUILD_WALL,
        BTN_BUILD_STRUCT,
        BTN_UPGRADE,
        BTN_CANCEL,

        NUM_BUTTONS
    };

public:
    PanelObjectActions();
    ~PanelObjectActions();

    void ClearObject();
    void SetObject(GameObject * obj);

    void SetButtonFunction(Button btnId, const std::function<void()> & f);

    void SetActionsEnabled(bool val);

private:
    std::array<ObjectActionButton *, NUM_BUTTONS> mButtons;

    GameObject * mObj = nullptr;
};

} // namespace game
