#include "GameObjects/Temple.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>

#include <cmath>

namespace game
{

Temple::Temple()
    : Structure(GameObject::TYPE_TEMPLE, GameObject::CAT_GENERIC, 3, 3)
{
    SetCanBeConquered(true);

    SetVisibilityLevel(4);

    SetImage();
    SetObjColors();
}

void Temple::SetInvestedResources(int money, int material, int blobs, int diamonds)
{
    const int maxMoney = 9999;
    const int maxMaterial = 9999;
    const int maxBlobs = 999;
    const int maxDiamonds = 999;

    if(money > maxMoney)
        money = maxMoney;

    if(material > maxMaterial)
        material = maxMaterial;

    if(blobs > maxBlobs)
        blobs = maxBlobs;

    if(diamonds > maxDiamonds)
        diamonds = maxDiamonds;

    const float maxTime = 300.f;
    const float minTime = 0.f;
    const float maxSuccess = 100.f;
    const float minSuccess = 0.f;

    // TIME
    const float timeInfluenceMoney = 35.f;
    const float timeInfluenceMaterial = 35.f;
    const float timeInfluenceBlobs = 15.f;
    const float timeInfluenceDiamonds = 15.f;

    const float timeCostMoney = timeInfluenceMoney / maxMoney;
    const float timeCostMaterial = timeInfluenceMaterial / maxMaterial;
    const float timeCostBlobs = timeInfluenceBlobs / maxBlobs;
    const float timeCostDiamonds = timeInfluenceDiamonds / maxDiamonds;

    const float timePerc = timeCostMoney * money + timeCostMaterial * material +
                           timeCostBlobs * blobs + timeCostDiamonds * diamonds;
    mExplorationTime = std::roundf(maxTime - maxTime * timePerc / 100.f);

    if(mExplorationTime < minTime)
        mExplorationTime = minTime;

    // SUCCESS
    const float successInfluenceMoney = 15.f;
    const float successInfluenceMaterial = 15.f;
    const float successInfluenceBlobs = 35.f;
    const float successInfluenceDiamonds = 35.f;

    const float successCostMoney = successInfluenceMoney / maxMoney;
    const float successCostMaterial = successInfluenceMaterial / maxMaterial;
    const float successCostBlobs = successInfluenceBlobs / maxBlobs;
    const float successCostDiamonds = successInfluenceDiamonds / maxDiamonds;

    const float successPerc = successCostMoney * money + successCostMaterial * material +
                           successCostBlobs * blobs + successCostDiamonds * diamonds;
    mExplorationSuccess = std::roundf(minSuccess + maxSuccess * successPerc / 100.f);

    if(mExplorationSuccess > maxSuccess)
        mExplorationSuccess = maxSuccess;
}

void Temple::UpdateGraphics()
{
    SetImage();

    SetObjColors();
}

void Temple::SetImage()
{
    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const unsigned int faction = GetFaction();
    const unsigned int sel = static_cast<unsigned int>(IsSelected());

    unsigned int texInd;

    if(NO_FACTION == faction)
        texInd = ID_STRUCT_TEMPLE + sel;
    else
        texInd = ID_STRUCT_TEMPLE_F1 + (faction * NUM_TEMPLE_SPRITES_PER_FAC) + sel;

    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texInd);

    isoObj->SetTexture(tex);
}

void Temple::SetObjColors()
{
    mObjColors.clear();

    mObjColors.push_back(0xbaa35eff);
    mObjColors.push_back(0xa18245ff);
    mObjColors.push_back(0x7d6535ff);
    mObjColors.push_back(0xa07c45ff);
    mObjColors.push_back(0x8f743dff);
    mObjColors.push_back(0x745e32ff);
}

} // namespace game
