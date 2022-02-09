#pragma once

#include <sgl/sgui/ButtonsGroup.h>

namespace game
{

class UnitsSelector : public sgl::sgui::ButtonsGroup
{
public:
    UnitsSelector(sgl::sgui::Widget * parent = nullptr);
};

} // namespace game
