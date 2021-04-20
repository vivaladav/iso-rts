#include "Widgets/PanelObjectActions.h"

#include "GameObjects/Base.h"
#include "GameObjects/Unit.h"
#include "Widgets/ObjectActionButton.h"

#include <core/event/KeyboardEvent.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

namespace game
{

PanelObjectActions::PanelObjectActions()
{
    using namespace lib::core;

    SetResizePolicy(ResizePolicy::DYNAMIC);

    // create all buttons
    mButtons[BTN_BUILD_UNIT] = new ObjectActionButton(ObjectActionButton::UNITS, "U", KeyboardEvent::KEY_U, this);
    mButtons[BTN_MOVE] = new ObjectActionButton(ObjectActionButton::MOVE, "M", KeyboardEvent::KEY_M, this);
    mButtons[BTN_ATTACK] = new ObjectActionButton(ObjectActionButton::ATTACK, "A", KeyboardEvent::KEY_A, this);
    mButtons[BTN_CONQUER] = new ObjectActionButton(ObjectActionButton::CONQUER, "C", KeyboardEvent::KEY_C, this);
    mButtons[BTN_BUILD_WALL] = new ObjectActionButton(ObjectActionButton::BUILD_WALL, "W", KeyboardEvent::KEY_W, this);
    mButtons[BTN_UPGRADE] = new ObjectActionButton(ObjectActionButton::UPGRADE, "U", KeyboardEvent::KEY_U, this);
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

    // ENABLE BUTTONS
    switch(mObj->GetObjectType())
    {
        case GameObjectType::OBJ_BASE:
        {
            mButtons[BTN_BUILD_UNIT]->SetVisible(true);
            mButtons[BTN_MOVE]->SetVisible(false);
            mButtons[BTN_ATTACK]->SetVisible(false);
            mButtons[BTN_CONQUER]->SetVisible(false);
            mButtons[BTN_BUILD_WALL]->SetVisible(false);
            mButtons[BTN_CANCEL]->SetVisible(true);
            // TODO handle upgrades
            mButtons[BTN_UPGRADE]->SetVisible(false);
        }
        break;

        case GameObjectType::OBJ_UNIT:
        {
            mButtons[BTN_BUILD_UNIT]->SetVisible(false);
            mButtons[BTN_MOVE]->SetVisible(true);
            mButtons[BTN_ATTACK]->SetVisible(true);
            mButtons[BTN_CONQUER]->SetVisible(true);
            mButtons[BTN_BUILD_WALL]->SetVisible(true);
            mButtons[BTN_CANCEL]->SetVisible(true);
            // TODO handle upgrades
            mButtons[BTN_UPGRADE]->SetVisible(false);
        }
        break;

        // object not supported -> hide all buttons
        default:
        {
            for(ObjectActionButton * btn : mButtons)
                btn->SetVisible(false);
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
    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();
    const int marginB = 50;

    const int panelX = (rendW - GetWidth()) * 0.5f;
    const int panelY = rendH - GetHeight() - marginB;

    SetPosition(panelX, panelY);
}

void PanelObjectActions::SetButtonFunction(Button btnId, const std::function<void()> & f)
{
    if(btnId < NUM_BUTTONS)
        mButtons[btnId]->SetOnClickFunction(f);
}

} // namespace game
