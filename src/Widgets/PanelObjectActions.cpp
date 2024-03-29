#include "Widgets/PanelObjectActions.h"

#include "GameObjects/WallGate.h"
#include "Widgets/ObjectActionButton.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Renderer.h>

namespace game
{

PanelObjectActions::PanelObjectActions(sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
{
    using namespace sgl::core;

    SetResizePolicy(ResizePolicy::DYNAMIC);

    // create all buttons
    mButtons[BTN_BUILD_UNIT] = new ObjectActionButton(ObjectActionButton::UNITS, "U", KeyboardEvent::KEY_U,
                                                      "Create a new unit", this);
    mButtons[BTN_MOVE] = new ObjectActionButton(ObjectActionButton::MOVE, "M", KeyboardEvent::KEY_M,
                                                "Move your unit", this);
    mButtons[BTN_ATTACK] = new ObjectActionButton(ObjectActionButton::ATTACK, "K", KeyboardEvent::KEY_K,
                                                  "Attack a target", this);
    mButtons[BTN_HEAL] = new ObjectActionButton(ObjectActionButton::HEAL, "H", KeyboardEvent::KEY_H,
                                                  "Heal/repair a target", this);
    mButtons[BTN_CONQUER_CELL] = new ObjectActionButton(ObjectActionButton::CONQUER_CELL, "C", KeyboardEvent::KEY_C,
                                                        "Conquer one or more cells", this);
    mButtons[BTN_BUILD_WALL] = new ObjectActionButton(ObjectActionButton::BUILD_WALL, "L", KeyboardEvent::KEY_L,
                                                      "Build a wall", this);
    mButtons[BTN_BUILD_STRUCT] = new ObjectActionButton(ObjectActionButton::BUILD_STRUCT, "B", KeyboardEvent::KEY_B,
                                                        "Build a structure", this);
    mButtons[BTN_UPGRADE] = new ObjectActionButton(ObjectActionButton::UPGRADE, "U", KeyboardEvent::KEY_U,
                                                   "Upgrade", this);
    mButtons[BTN_OPEN_GATE] = new ObjectActionButton(ObjectActionButton::OPEN_GATE, "G", KeyboardEvent::KEY_G,
                                                     "Open the game", this);
    mButtons[BTN_CLOSE_GATE] = new ObjectActionButton(ObjectActionButton::CLOSE_GATE, "G", KeyboardEvent::KEY_G,
                                                      "Close the gate", this);
    mButtons[BTN_CANCEL] = new ObjectActionButton(ObjectActionButton::CANCEL, "X", KeyboardEvent::KEY_X,
                                                  "Cancel current action", this);
}

PanelObjectActions::~PanelObjectActions()
{

}

void PanelObjectActions::ClearObject()
{

}

void PanelObjectActions::SetObject(GameObject * obj)
{
    mObj = obj;

    // reset all buttons
    for(ObjectActionButton * btn : mButtons)
        btn->SetVisible(false);

    // cancel is always visible
    mButtons[BTN_CANCEL]->SetVisible(true);

    // ENABLE BUTTONS
    const GameObjectTypeId objType = mObj->GetObjectType();

    if(objType == GameObject::TYPE_BASE)
    {
        mButtons[BTN_BUILD_UNIT]->SetVisible(true);

        // TODO handle upgrades
    }
    else if(mObj->GetObjectCategory() == GameObject::CAT_UNIT)
    {
        mButtons[BTN_MOVE]->SetVisible(true);
        mButtons[BTN_ATTACK]->SetVisible(true);
        mButtons[BTN_HEAL]->SetVisible(true);
        mButtons[BTN_CONQUER_CELL]->SetVisible(true);
        mButtons[BTN_BUILD_WALL]->SetVisible(true);
        mButtons[BTN_BUILD_STRUCT]->SetVisible(true);

        // TODO handle upgrades
    }
    else if(objType == GameObject::TYPE_WALL_GATE)
    {
        auto gate = static_cast<WallGate *>(mObj);

        if(gate->IsOpen())
            mButtons[BTN_CLOSE_GATE]->SetVisible(true);
        else
            mButtons[BTN_OPEN_GATE]->SetVisible(true);
    }

    // object not supported -> hide all buttons
    else
        mButtons[BTN_CANCEL]->SetVisible(false);

    // POSITION BUTTONS
    const int marginH = 15;
    int x = 0;

    for(ObjectActionButton * btn : mButtons)
    {
        if(btn->IsVisible())
        {
            btn->SetX(x);

            x += btn->GetWidth() + marginH;
        }
    }

    // update position
    const int rendW = sgl::graphic::Renderer::Instance()->GetWidth();
    const int rendH = sgl::graphic::Renderer::Instance()->GetHeight();
    const int marginB = 100;

    const int panelX = (rendW - GetWidth()) * 0.5f;
    const int panelY = rendH - GetHeight() - marginB;

    SetPosition(panelX, panelY);
}

void PanelObjectActions::SetButtonFunction(Button btnId, const std::function<void()> & f)
{
    if(btnId < NUM_BUTTONS)
        mButtons[btnId]->AddOnClickFunction(f);
}

void PanelObjectActions::SetActionsEnabled(bool val)
{
    for(unsigned int i = 0; i < static_cast<unsigned int>(BTN_CANCEL); ++i)
        mButtons[i]->SetEnabled(val);
}

} // namespace game
