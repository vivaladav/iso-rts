#include "GameObjects/ResourceStorage.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

ResourceStorage::ResourceStorage(ResourceType typeRes, int rows, int cols)
    : Structure(OBJ_RES_STORAGE, rows, cols)
    , mTypeRes(typeRes)
{
    SetCanBeConquered(true);

    // set capacity based on resource
    const int capacities[NUM_RESOURCES] = { 500, 250, 150, 100 };
    mCapacity = capacities[mTypeRes];

    SetImage();
}

void ResourceStorage::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void ResourceStorage::OnLinkedChanged()
{
    GameObject::OnLinkedChanged();

    Player * p = GetOwner();

    if(nullptr == p)
        return ;

    const int diff = IsLinked() ? mCapacity : -mCapacity;

    const Player::Stat statIds[NUM_RESOURCES] =
    {
        Player::Stat::ENERGY,
        Player::Stat::MATERIAL,
        Player::Stat::DIAMONDS,
        Player::Stat::BLOBS
    };

    p->SumResourceMax(statIds[mTypeRes], diff);
}

void ResourceStorage::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const Player * owner = GetOwner();
    const unsigned int faction = owner ? owner->GetFaction() : NO_FACTION;
    const unsigned int sel = static_cast<unsigned int>(IsSelected());

    unsigned int texId = 0;

    if(RES_ENERGY == mTypeRes)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_ENERGY_F1 + (faction * NUM_ENE_STO_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_STORAGE_ENERGY;
    }
    else if(RES_MATERIAL1 == mTypeRes)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_MATERIAL_F1 + (faction * NUM_ENE_STO_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_STORAGE_MATERIAL;
    }
    else if(RES_DIAMONDS == mTypeRes)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_DIAMONDS_F1 + (faction * NUM_ENE_STO_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_STORAGE_DIAMONDS;
    }
    else if(RES_BLOBS == mTypeRes)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_STORAGE_BLOBS_F1 + (faction * NUM_ENE_STO_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_STORAGE_BLOBS;
    }

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texId);
    isoObj->SetTexture(tex);
}

} // namespace game

