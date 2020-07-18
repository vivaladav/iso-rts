
#include "GameButton.h"

#include <graphic/Image.h>

namespace game
{

GameButton::GameButton(lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
{
}

GameButton::GameButton(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
                       const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
                       lib::sgui::Widget * parent)
    : lib::sgui::PushButton(parent)
{
    SetData(bgFiles, labelsColor, NORMAL);
}

GameButton::~GameButton()
{
    for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        delete mBackgrounds[i];
}

void GameButton::SetData(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
                         const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
                         int state)
{
    // init background data
    for(unsigned int i = 0; i < NUM_VISUAL_STATES; ++i)
        mBackgrounds[i] = new lib::graphic::Image(bgFiles[i]);

    // init label data
    mLabelsColor = labelsColor;

    SetElements(state);
}

void GameButton::HandleStateEnabled()
{
    if(IsChecked())
        SetElements(CHECKED);
    else
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

    if(IsCheckable())
        SetElements(CHECKED);
    else
        SetElements(PUSHED);
}

void GameButton::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    PushButton::HandleMouseButtonUp(event);

    if(!IsEnabled())
        return ;

    if(IsCheckable())
        SetElements(IsChecked() ? CHECKED : NORMAL);
    else
        SetElements(NORMAL);
}

void GameButton::HandleMouseOver()
{
    if(!IsEnabled() || IsChecked())
        return ;

    SetElements(MOUSE_OVER);
}

void GameButton::HandleMouseOut()
{
    if(!IsEnabled() || IsChecked())
        return ;

    SetElements(NORMAL);
}

void GameButton::SetElements(int index)
{
    if(mState == index)
        return ;

    mState = index;

    SetCurrBg(mBackgrounds[index]);

    SetLabelColor(mLabelsColor[index]);
}

} // namespace game

