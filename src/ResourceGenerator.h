#pragma once

#include "GameObject.h"

namespace game
{

enum ResourceType
{
    ENERGY,

    NUM_RESOURCES,

    INVALID
};

class ResourceGenerator : public GameObject
{
public:
    ResourceGenerator(int owner, ResourceType type);

    int GetOutput() const;

    ResourceType GetResourceType() const;

protected:
    void UpdateImageId() override;

private:
    void SetImageId();

    void UpdateOutput();

private:
    ResourceType mResType = INVALID;

    int mOutput = 0;
};

inline int ResourceGenerator::GetOutput() const { return mOutput; }

inline ResourceType ResourceGenerator::GetResourceType() const { return mResType; }

} // namespace game

