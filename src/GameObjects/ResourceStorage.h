#pragma once

#include "Structure.h"

namespace game
{

enum ResourceType : unsigned int;


class ResourceStorage : public Structure
{
public:
    ResourceStorage(ResourceType typeRes, int rows, int cols);

    ResourceType GetResourceType() const;

protected:
    void UpdateGraphics() override;

    void OnLinkedChanged() override;

private:
    void SetImage();

private:
    ResourceType mTypeRes;

    int mCapacity = 0;
};

inline ResourceType ResourceStorage::GetResourceType() const { return mTypeRes; }

} // namespace game

