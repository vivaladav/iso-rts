#include "GameObjects/ResourceGenerator.h"

#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

ResourceGenerator::ResourceGenerator(ResourceType type, int rows, int cols)
    : GameObject(OBJ_RES_GEN, -1, rows, cols)
    , mResType(type)
{
    SetStructure(true);
    SetCanBeConquered(true);

    SetImage();

    UpdateOutput();
}

void ResourceGenerator::UpdateImage()
{
    SetImage();
}

void ResourceGenerator::SetImage()
{
    const int owner = GetOwner();

    auto * tm = lib::graphic::TextureManager::Instance();

    lib::graphic::Texture * tex = nullptr;

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    if(ENERGY == mResType)
    {
        // not visible
        if(!IsVisible())
            tex = tm->GetTexture("data/img/energy_source.png");
        // visible
        else
        {
            if(-1 == owner)
                tex = tm->GetTexture("data/img/energy_source.png");
            else
            {
                const char * filesFactions[] =
                {
                    "data/img/energy_source-f1.png",
                    "data/img/energy_source-f2.png",
                    "data/img/energy_source-f3.png"
                };

                tex = tm->GetTexture(filesFactions[owner]);
            }
        }
    }
    else if(MATERIAL1 == mResType)
    {
        // not visible
        if(!IsVisible())
            tex = tm->GetTexture("data/img/material_source.png");
        // visible
        else
        {
            GetIsoObject()->SetColor(COLOR_VIS);

            if(-1 == owner)
                tex = tm->GetTexture("data/img/material_source.png");
            else
            {
                const char * filesFactions[] =
                {
                    "data/img/material_source-f1.png",
                    "data/img/material_source-f2.png",
                    "data/img/material_source-f3.png"
                };

                tex = tm->GetTexture(filesFactions[owner]);
            }
        }
    }
    // unknown resource type
    else
        tex = tm->GetTexture("data/img/obj_null.png");

    isoObj->SetTexture(tex);
}

void ResourceGenerator::UpdateOutput()
{
    if(ENERGY == mResType)
        mOutput = 20;
    else if(MATERIAL1 == mResType)
        mOutput = 2;
    // default
    else
        mOutput = 1;
}

} // namespace game

