#pragma once

#include <sgui/Widget.h>

#include <functional>
#include <vector>

namespace lib
{
    namespace sgui { class Image; }
}

namespace game
{

class Player;

enum UnitType : unsigned int;

class DialogNewElement : public lib::sgui::Widget
{
public:
    DialogNewElement(Player * player);
    ~DialogNewElement();

    void SetFunctionOnBuild(const std::function<void()> & f);
    void SetFunctionOnClose(const std::function<void()> & f);

private:
    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

private:

};

} // namespace game
