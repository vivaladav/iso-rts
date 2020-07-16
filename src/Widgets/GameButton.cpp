
#include "GameButton.h"

#include <graphic/Image.h>

namespace game
{

GameButton::GameButton(const char * text,
                       const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
                       const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
                       lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
{
    // init background data
    for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        mBackgrounds[i] = new lib::graphic::Image(bgFiles[i]);

    // init label data
    SetLabel(text);

    mLabelsColor = labelsColor;

    // set initial elements
    SetElements(NORMAL);
}

GameButton::~GameButton()
{
    for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        delete mBackgrounds[i];
}

void GameButton::HandleStateEnabled()
{
    SetElements(NORMAL);
}
void GameButton::HandleStateDisabled()
{
     SetElements(DISABLED);
}

void GameButton::HandleMouseButtonDown(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonDown(event);

    if(!IsEnabled())
        return ;

    SetElements(PUSHED);
}

void GameButton::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    if(!IsEnabled())
        return ;

    SetElements(NORMAL);
}

void GameButton::HandleMouseOver()
{
    SetElements(MOUSE_OVER);
}

void GameButton::HandleMouseOut()
{
    SetElements(NORMAL);
}

void GameButton::SetElements(int index)
{
    SetCurrBg(mBackgrounds[index]);

    SetLabelColor(mLabelsColor[index]);
}

} // namespace game

