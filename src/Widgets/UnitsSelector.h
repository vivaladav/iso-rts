#pragma once

#include <sgui/ButtonsGroup.h>

namespace game
{

class UnitsSelector : public lib::sgui::ButtonsGroup
{
public:
    UnitsSelector(lib::sgui::Widget * parent = nullptr);
};

} // namespace game
