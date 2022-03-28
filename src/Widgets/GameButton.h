#pragma once

#include <sgl/sgui/PushButton.h>

#include <array>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Texture;
    }
}

namespace game
{

class GameButton : public sgl::sgui::PushButton
{
public:
    GameButton(sgl::sgui::Widget * parent);
    GameButton(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
               const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
               sgl::sgui::Widget * parent);
    GameButton(const char * spriteFile,
               const std::array<unsigned int, NUM_VISUAL_STATES> & spriteIds,
               const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor,
               sgl::sgui::Widget * parent);

protected:
    void SetData(const std::array<const char *, NUM_VISUAL_STATES> & bgFiles,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor);

    void SetData(const char * spriteFile,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & spriteIds,
                 const std::array<unsigned int, NUM_VISUAL_STATES> & labelsColor);

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;

private:
    std::array<sgl::graphic::Texture *, NUM_VISUAL_STATES> mBackgrounds;
    std::array<unsigned int, NUM_VISUAL_STATES> mLabelsColor;

    sgl::graphic::Image * mBody = nullptr;
};

} // namespace game
