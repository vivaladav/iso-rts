#include "Widgets/WidgetCover.h"

namespace game
{

WidgetCover::WidgetCover(const sgl::sgui::Widget * target)
    : WidgetCover(target, nullptr)
{
}

WidgetCover::WidgetCover(const sgl::sgui::Widget * target, sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
{
    SetSize(target->GetWidth(), target->GetHeight());
}

} // namespace game