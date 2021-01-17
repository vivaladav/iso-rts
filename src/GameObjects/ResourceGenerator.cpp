#include "GameObjects/ResourceGenerator.h"

#include "IsoObject.h"

#include <graphic/TextureManager.h>

namespace game
{

ResourceGenerator::ResourceGenerator(ResourceType type, int rows, int cols)
    : GameObject(OBJ_RES_GEN, -1, rows, cols)
    , mResType(type)
{
    mVisRadius = 2;

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
    else if(MATERIAL1 == mResType)
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
    else
        tex = tm->GetTexture("data/img/obj_null.png");

    mIsoObj->SetTexture(tex);
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

