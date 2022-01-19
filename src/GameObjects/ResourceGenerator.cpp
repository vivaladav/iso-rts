#include "GameObjects/ResourceGenerator.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <graphic/TextureManager.h>

namespace game
{

ResourceGenerator::ResourceGenerator(ResourceGeneratorType typeGen, ResourceType typeRes, int rows, int cols)
    : Structure(OBJ_RES_GEN, rows, cols)
    , mTypeGen(typeGen)
    , mTypeRes(typeRes)
{
    SetCanBeConquered(true);

    SetImage();

    UpdateOutput();
}

void ResourceGenerator::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void ResourceGenerator::SetImage()
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

    if(RESG_ENERGY == mTypeGen)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_GEN_ENERGY_F1 + (faction * NUM_ENE_GEN_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_GEN_ENERGY;
    }
    else if(RESG_MATERIAL == mTypeGen)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_GEN_MATERIAL_F1 + (faction * NUM_MAT_GEN_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_GEN_MATERIAL;
    }
    else if(RESG_ENERGY_SOLAR == mTypeGen)
    {
        if(faction != NO_FACTION && IsVisible())
            texId = ID_STRUCT_SOLAR_PANEL_F1 + (faction * NUM_SOLAR_PANEL_SPRITES_PER_FAC) + sel;
        else
            texId = ID_STRUCT_SOLAR_PANEL;
    }

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texId);
    isoObj->SetTexture(tex);
}

void ResourceGenerator::UpdateOutput()
{
    if(RESG_ENERGY == mTypeGen)
        mOutput = 30;
    else if(RESG_MATERIAL == mTypeGen)
        mOutput = 2;
    else if(RESG_ENERGY_SOLAR == mTypeGen)
        mOutput = 4;
    // default
    else
        mOutput = 1;
}

} // namespace game

