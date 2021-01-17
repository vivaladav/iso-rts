#include "GameObjects/ResourceGenerator.h"

#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

ResourceGenerator::ResourceGenerator(ResourceType type, int rows, int cols)
    : GameObject(OBJ_RES_GEN, -1, rows, cols)
    , mResType(type)
{
    SetVisibilityRadius(2);
    SetStructure(true);

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

    if(ENERGY == mResType)
    {
        // not visible
        if(!IsVisible())
            tex = tm->GetTexture("data/img/energy_source-fow.png");
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
            tex = tm->GetTexture("data/img/material_source-fow.png");
        // visible
        else
        {
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

    GetIsoObject()->SetTexture(tex);
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

