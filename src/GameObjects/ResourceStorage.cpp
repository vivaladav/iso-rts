#include "GameObjects/ResourceStorage.h"

#include "Game.h"
#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <sgl/graphic/TextureManager.h>
#include <sgl/utilities/BinaryFile.h>

#include <cmath>

namespace game
{

ResourceStorage::ResourceStorage(const ObjectData & data, const ObjectInitData & initData)
    : Structure(data, initData)
{
    if(ObjectData::TYPE_RES_STORAGE_BLOBS == data.GetType())
        mResource = ER_BLOBS;
    else if(ObjectData::TYPE_RES_STORAGE_DIAMONDS == data.GetType())
        mResource = ER_DIAMONDS;
    else if(ObjectData::TYPE_RES_STORAGE_ENERGY == data.GetType())
        mResource = ER_ENERGY;
    else if(ObjectData::TYPE_RES_STORAGE_MATERIAL == data.GetType())
        mResource = ER_MATERIAL;
    else
    {
        mResource = ER_NULL;
        return;
    }

    SetCanBeConquered(true);

    UpdateCapacity();

    SetImage();
}

bool ResourceStorage::Load(sgl::utilities::BinaryFile & bf)
{
    const bool res = Structure::Load(bf);

    if(!res)
        return false;

    // flags
    mCapacity = bf.ReadInt();

    return true;
}

bool ResourceStorage::Save(sgl::utilities::BinaryFile & bf) const
{
    const bool res = Structure::Save(bf);

    if(!res)
        return false;

    // flags
    bf.WriteInt(mCapacity);

    return true;
}

void ResourceStorage::UpdateGraphics()
{
    SetImage();
}

void ResourceStorage::OnLinkedChanged()
{
    Structure::OnLinkedChanged();

    if(NO_FACTION == GetFaction())
        return ;

    const int diff = IsLinked() ? mCapacity : -mCapacity;

    Player * p = GetOwner();
    p->SumResourceMax(mResource, diff);
}

void ResourceStorage::OnAttributeChanged()
{
    UpdateCapacity();
}

void ResourceStorage::UpdateCapacity()
{
    const int oldCapacity = mCapacity;

    UpdateCapacityValue();

    // update owner's capacity
    if(oldCapacity != mCapacity && IsLinked())
    {
        const int diff = mCapacity - oldCapacity;

        Player * p = GetOwner();
        p->SumResourceMax(mResource, diff);
    }
}

void ResourceStorage::UpdateCapacityValue()
{
    Player * p = GetOwner();

    // set max capacity value
    const bool mainRes = ER_ENERGY == mResource || ER_MATERIAL == mResource;
    const float maxCapacity = mainRes ? 2000.f : 1000.f;

    // define upgrade multiplier
    float mult = 1.f;

    if(p != nullptr)
    {
        if(ER_ENERGY == mResource)
            mult = p->GetStorageEnergyMult();
        else if(ER_MATERIAL == mResource)
            mult = p->GetStorageMaterialMult();
        else if(ER_DIAMONDS == mResource)
            mult = p->GetStorageDiamondsMult();
        else if(ER_BLOBS == mResource)
            mult = p->GetStorageBlobsMult();
    }

    // update capacity value
    mCapacity = std::roundf(mult * maxCapacity * GetAttribute(OBJ_ATT_STORAGE) / MAX_STAT_FVAL);
}

void ResourceStorage::SetImage()
{
    auto * tm = sgl::graphic::TextureManager::Instance();

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const unsigned int faction = GetFaction();

    unsigned int texId = 0;

    if(ER_ENERGY == mResource)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_ENERGY_F1 + faction;
        else
            texId = ID_STRUCT_STORAGE_ENERGY;
    }
    else if(ER_MATERIAL == mResource)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_MATERIAL_F1 + faction ;
        else
            texId = ID_STRUCT_STORAGE_MATERIAL;
    }
    else if(ER_DIAMONDS == mResource)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_DIAMONDS_F1 + faction;
        else
            texId = ID_STRUCT_STORAGE_DIAMONDS;
    }
    else if(ER_BLOBS == mResource)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_BLOBS_F1 + faction;
        else
            texId = ID_STRUCT_STORAGE_BLOBS;
    }

    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texId);
    isoObj->SetTexture(tex);
}

} // namespace game

