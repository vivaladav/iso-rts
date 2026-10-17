#pragma once

#include <sgl/sgui/Widget.h>

namespace game
{

class WidgetCover : public sgl::sgui::Widget
{
public:
    WidgetCover(const sgl::sgui::Widget * target);
    WidgetCover(const sgl::sgui::Widget * target, sgl::sgui::Widget * parent);
};

} // namespace game
