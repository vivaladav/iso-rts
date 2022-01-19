#pragma once

#include "Structure.h"

namespace game
{

enum ResourceGeneratorType : unsigned int
{
    RESG_ENERGY,
    RESG_ENERGY_SOLAR,
    RESG_MATERIAL,

    NUM_RES_GEN_TYPES
};

enum ResourceType : unsigned int;


class ResourceGenerator : public Structure
{
public:
    ResourceGenerator(ResourceGeneratorType typeGen, ResourceType typeRes, int rows, int cols);

    int GetOutput() const;

    ResourceType GetResourceType() const;

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void UpdateOutput();

private:
    ResourceGeneratorType mTypeGen;
    ResourceType mTypeRes;

    int mOutput = 0;
};

inline int ResourceGenerator::GetOutput() const { return mOutput; }

inline ResourceType ResourceGenerator::GetResourceType() const { return mTypeRes; }

} // namespace game

