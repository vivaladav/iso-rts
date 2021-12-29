#pragma once

#include <sgui/PushButton.h>

#include <array>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

class GameButton : public lib::sgui::PushButton
{
public:
    GameButton(lib::sgui::Widget * parent);
    GameButton(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
               const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
               lib::sgui::Widget * parent);
    GameButton(const char * spriteFile,
               const std::array<unsigned int, NUM_VISUAL_STATES> & spriteIds,
               const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
               lib::sgui::Widget * parent);

    ~GameButton();

protected:
    void SetData(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor);

    void SetData(const char * spriteFile,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & spriteIds,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor);

    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

private:
    std::array<lib::graphic::Image *, NUM_VISUAL_STATES> mBackgrounds;
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;
};

} // namespace game
