#pragma once

#include "GameObject.h"

namespace game
{

enum ResourceType : unsigned int
{
    ENERGY,
    MATERIAL1,

    NUM_RESOURCES,

    INVALID
};

class ResourceGenerator : public GameObject
{
public:
    ResourceGenerator(ResourceType type, int rows, int cols);

    int GetOutput() const;

    ResourceType GetResourceType() const;

protected:
    void UpdateImage() override;

private:
    void SetImage();

    void UpdateOutput();

private:
    ResourceType mResType = INVALID;

    int mOutput = 0;
};

inline int ResourceGenerator::GetOutput() const { return mOutput; }

inline ResourceType ResourceGenerator::GetResourceType() const { return mResType; }

} // namespace game

