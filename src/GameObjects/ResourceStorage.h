#pragma once

#include "Structure.h"

namespace game
{

enum ExtendedResource : unsigned int;

class ResourceStorage : public Structure
{
public:
    ResourceStorage(const ObjectData & data, const ObjectInitData & initData);

    bool Load(sgl::utilities::BinaryFile & bf) override;
    bool Save(sgl::utilities::BinaryFile & bf) const override;

    ExtendedResource GetExtendedResource() const;

    void OnCapacityUpgraded();

private:
    void UpdateGraphics() override;

    void OnLinkedChanged() override;
    void OnAttributeChanged() override;

    void UpdateCapacity();
    void UpdateCapacityValue();

    void SetImage();

private:
    ExtendedResource mResource;

    int mCapacity = 0;
};

inline ExtendedResource ResourceStorage::GetExtendedResource() const { return mResource; }

inline void ResourceStorage::OnCapacityUpgraded() { UpdateCapacity(); }

} // namespace game

