#include "Widgets/UnitsSelector.h"

#include "Widgets/ButtonUnitsSelector.h"

namespace game
{

UnitsSelector::UnitsSelector(lib::sgui::Widget * parent)
    : lib::sgui::ButtonsGroup(lib::sgui::ButtonsGroup::HORIZONTAL, parent)
{
    AddButton(new ButtonUnitsSelector("1"));
    AddButton(new ButtonUnitsSelector("2"));
    AddButton(new ButtonUnitsSelector("3"));
    AddButton(new ButtonUnitsSelector("4"));
}

} // namespace game
