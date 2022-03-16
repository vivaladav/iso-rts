#include "Widgets/PanelObjectActions.h"

#include "GameObjects/Base.h"
#include "GameObjects/Unit.h"
#include "GameObjects/WallGate.h"
#include "Widgets/ObjectActionButton.h"

#include <sgl/core/event/KeyboardEvent.h>
#include <sgl/graphic/Image.h>
#include <sgl/graphic/Renderer.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>

namespace game
{

PanelObjectActions::PanelObjectActions()
{
    using namespace sgl::core;

    SetResizePolicy(ResizePolicy::DYNAMIC);

    // create all buttons
    mButtons[BTN_BUILD_UNIT] = new ObjectActionButton(ObjectActionButton::UNITS, "U", KeyboardEvent::KEY_U, this);
    mButtons[BTN_MOVE] = new ObjectActionButton(ObjectActionButton::MOVE, "M", KeyboardEvent::KEY_M, this);
    mButtons[BTN_ATTACK] = new ObjectActionButton(ObjectActionButton::ATTACK, "A", KeyboardEvent::KEY_A, this);
    mButtons[BTN_CONQUER_CELL] = new ObjectActionButton(ObjectActionButton::CONQUER_CELL, "C", KeyboardEvent::KEY_C, this);
    mButtons[BTN_BUILD_WALL] = new ObjectActionButton(ObjectActionButton::BUILD_WALL, "W", KeyboardEvent::KEY_W, this);
    mButtons[BTN_BUILD_STRUCT] = new ObjectActionButton(ObjectActionButton::BUILD_STRUCT, "B", KeyboardEvent::KEY_B, this);
    mButtons[BTN_UPGRADE] = new ObjectActionButton(ObjectActionButton::UPGRADE, "U", KeyboardEvent::KEY_U, this);
    mButtons[BTN_OPEN_GATE] = new ObjectActionButton(ObjectActionButton::OPEN_GATE, "G", KeyboardEvent::KEY_G, this);
    mButtons[BTN_CLOSE_GATE] = new ObjectActionButton(ObjectActionButton::CLOSE_GATE, "G", KeyboardEvent::KEY_G, this);
    mButtons[BTN_CANCEL] = new ObjectActionButton(ObjectActionButton::CANCEL, "X", KeyboardEvent::KEY_X, this);
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
    switch(mObj->GetObjectType())
    {
        case GameObjectType::OBJ_BASE:
        {
            mButtons[BTN_BUILD_UNIT]->SetVisible(true);

            // TODO handle upgrades
        }
        break;

        case GameObjectType::OBJ_UNIT:
        {
            mButtons[BTN_MOVE]->SetVisible(true);
            mButtons[BTN_ATTACK]->SetVisible(true);
            mButtons[BTN_CONQUER_CELL]->SetVisible(true);
            mButtons[BTN_BUILD_WALL]->SetVisible(true);
            mButtons[BTN_BUILD_STRUCT]->SetVisible(true);

            // TODO handle upgrades
        }
        break;

        case GameObjectType::OBJ_WALL_GATE:
        {
            auto gate = static_cast<WallGate *>(mObj);

            if(gate->IsOpen())
                mButtons[BTN_CLOSE_GATE]->SetVisible(true);
            else
                mButtons[BTN_OPEN_GATE]->SetVisible(true);
        }
        break;

        // object not supported -> hide all buttons
        default:
        {
            mButtons[BTN_CANCEL]->SetVisible(false);
        }
        break;
    }

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
