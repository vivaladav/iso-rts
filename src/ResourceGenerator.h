#pragma once

#include "GameObject.h"

namespace game
{

enum ResourceType
{
    ENERGY,

    NUM_RESOURCES
};

class ResourceGenerator : public GameObject
{
public:
    ResourceGenerator(int owner, ResourceType);

protected:
    void UpdateImageId() override;

private:
    void SetImageId();
};

} // namespace game

