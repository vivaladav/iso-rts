#pragma once

#include "GameObject.h"

namespace game
{

class Base : public GameObject
{
public:
    Base(int owner);

protected:
    void UpdateImageId() override;

private:
    void SetImageId();
};

} // namespace game
