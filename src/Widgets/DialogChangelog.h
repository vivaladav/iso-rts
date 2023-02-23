#pragma once

#include <sgl/sgui/AbstractButton.h>

#include <functional>

namespace sgl
{
    namespace graphic
    {
        class Image;
        class Text;
    }

    namespace sgui { class TextArea; }
}

namespace game
{

class ButtonCloseChangelog;
class ChangelogArea;

// ===== BUTTON CHANGELOG =====
class ButtonChangelog : public sgl::sgui::AbstractButton
{
public:
    ButtonChangelog();

private:
    void HandlePositionChanged() override;

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;

    void PositionElements();
    void UpdateLabel(sgl::sgui::AbstractButton::VisualState state);

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::sgui::TextArea * mLabel = nullptr;
};

// ===== DIALOG CHANGELOG =====
class DialogChangelog : public sgl::sgui::Widget
{
public:
    DialogChangelog();

    void SetContent(sgl::sgui::Widget * cont);

    unsigned int AddOnCloseClickFunction(const std::function<void()> & f);

private:
    void HandlePositionChanged() override;

    void PositionElements();

private:
    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Text * mTitle = nullptr;
    ChangelogArea * mContArea = nullptr;
    ButtonCloseChangelog * mButtonClose = nullptr;
};

} // namespace game
