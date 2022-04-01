#pragma once

#include <sgl/sgui/ComboBox.h>
#include <sgl/sgui/ComboBoxItem.h>

#include <array>

namespace sgl
{
    namespace graphic { class Image; }
}

namespace game
{

// ==== COMBO BOX ITEM ====
class TestComboBoxItem : public sgl::sgui::ComboBoxItem
{
public:
    TestComboBoxItem(const char * txt);

private:
    void HandlePositionChanged() override;

    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;

    void PositionElements();

private:
    sgl::graphic::Image * mBody = nullptr;
    sgl::graphic::Text * mText = nullptr;
};

// ==== COMBO BOX ====
class TestComboBox : public sgl::sgui::ComboBox
{

public:
    TestComboBox(sgl::sgui::Widget * parent);

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;
};

} // namespace game
