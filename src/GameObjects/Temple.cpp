#include "GameObjects/Temple.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"

#include <sgl/graphic/TextureManager.h>
#include <sgl/utilities/LoadedDie.h>
#include <sgl/utilities/UniformDistribution.h>

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
    const int maxMoney = GetMaxInvestableMoney();
    const int maxMaterial = GetMaxInvestableMaterial();
    const int maxBlobs = GetMaxInvestableBlobs();
    const int maxDiamonds = GetMaxInvestableDiamonds();

    if(money > maxMoney)
        money = maxMoney;

    if(material > maxMaterial)
        material = maxMaterial;

    if(blobs > maxBlobs)
        blobs = maxBlobs;

    if(diamonds > maxDiamonds)
        diamonds = maxDiamonds;

    const float maxTime = 60.f;
    const float minTime = 0.f;
    const float maxSuccess = 100.f;
    const float minSuccess = 0.f;

    // TIME
    const float timeInfluenceMoney = 40.f;
    const float timeInfluenceMaterial = 30.f;
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
    const float successInfluenceMoney = 25.f;
    const float successInfluenceMaterial = 15.f;
    const float successInfluenceBlobs = 30.f;
    const float successInfluenceDiamonds = 30.f;

    const float successCostMoney = successInfluenceMoney / maxMoney;
    const float successCostMaterial = successInfluenceMaterial / maxMaterial;
    const float successCostBlobs = successInfluenceBlobs / maxBlobs;
    const float successCostDiamonds = successInfluenceDiamonds / maxDiamonds;

    const float successPerc = successCostMoney * money + successCostMaterial * material +
                           successCostBlobs * blobs + successCostDiamonds * diamonds;
    mExplorationSuccess = std::roundf(minSuccess + maxSuccess * successPerc / 100.f);

    if(mExplorationSuccess > maxSuccess)
        mExplorationSuccess = maxSuccess;

    // TEMP DEBUG
    mExplorationTime = 2;
    mExplorationSuccess = 50;
}

void Temple::Explore()
{
    // DEBUG GOOD
    // mOutcomeCat = EXP_OUTC_GOOD;
    // DecideRewards();

    // DEBUG BAD
    // mOutcomeCat = EXP_OUTC_BAD;
    // DecidePunishments();

    // DEBUG NOTHING
    //mOutcomeCat = EXP_OUTC_NOTHING;

    const float probSuccess = mExplorationSuccess;
    const float probFail = 100.f - mExplorationSuccess;
    sgl::utilities::LoadedDie die({ EXP_OUTC_GOOD, EXP_OUTC_BAD }, { probSuccess, probFail });

    mOutcomeCat = static_cast<ExplorationOutcomeCategory>(die.GetNextValue());

    // GOOD -> positive reward
    if(mOutcomeCat == EXP_OUTC_GOOD)
    {
            DecideRewards();
        return ;
    }

    // BAD route -> decide if NOTHING or BAD
    sgl::utilities::LoadedDie die2({ EXP_OUTC_NOTHING, EXP_OUTC_BAD }, { probSuccess, probFail });

    mOutcomeCat = static_cast<ExplorationOutcomeCategory>(die2.GetNextValue());

    // NOTHING
    if(mOutcomeCat == EXP_OUTC_NOTHING)
    {
        mOutcome1 = EXP_OUT_NULL;
        mOutcome2 = EXP_OUT_NULL;
    }
    // BAD
    else
        DecidePunishments();
}

const char * Temple::GetExplorationOutcomeString(ExplorationOutcome o) const
{
    if(o >= NUM_EXPLORATION_OUTCOMES)
        return nullptr;

    static const char * strings[NUM_EXPLORATION_OUTCOMES] =
    {
        // -- REWARDS --
        // INCREASE EXISTING RESOURCES
        "Your money will be increased 10 times.",
        "Your energy and material will be maximized.",
        "Your blobs and diamonds will be maximized.",
        "All your resources will be maximized.",
        // INCREASE PRODUCTION
        "The production of all your current energy generators will double.",
        "The production of all your current material generators will double.",
        // MAXIMIZE COLLECTIBLES
        "All the existing blobs on the map will be maximized.",
        "All the existing diamonds on the map will be maximized.",

        // -- PUNISHMENTS --
        // DECREASE EXISTING RESOURCES
        "Your money will go down to `0.",
        "Your energy and material will go down to 0.",
        "Your blobs and diamonds will go down to 0.",
        "All your resources will go down to 0.",
        // DECREASE PRODUCTION
        "The production of all your current energy generators will halve.",
        "The production of all your current material generators will halve.",
        // MINIMIZE COLLECTIBLES
        "All the existing blobs on the map will be minimized.",
        "All the existing diamonds on the map will be minimized.",
    };

    return strings[o];
}

void Temple::DecideRewards()
{
    sgl::utilities::UniformDistribution dist(FIRST_EXP_REW, LAST_EXP_REW);

    mOutcome1 = static_cast<ExplorationOutcome>(dist.GetNextValue());
    mOutcome2 = static_cast<ExplorationOutcome>(dist.GetNextValue());

    // same result -> try again
    if(mOutcome2 == mOutcome1)
        mOutcome2 = static_cast<ExplorationOutcome>(dist.GetNextValue());

    // same result again -> pick next
    if(mOutcome2 == mOutcome1)
        mOutcome2 = static_cast<ExplorationOutcome>(FIRST_EXP_REW + ((mOutcome2 + 1) % (LAST_EXP_REW + 1)));
}

void Temple::DecidePunishments()
{
    sgl::utilities::UniformDistribution dist(FIRST_EXP_PUN, LAST_EXP_PUN);

    mOutcome1 = static_cast<ExplorationOutcome>(dist.GetNextValue());
    mOutcome2 = static_cast<ExplorationOutcome>(dist.GetNextValue());

    // same result -> try again
    if(mOutcome2 == mOutcome1)
        mOutcome2 = static_cast<ExplorationOutcome>(dist.GetNextValue());

    // same result again -> pick next
    if(mOutcome2 == mOutcome1)
        mOutcome2 = static_cast<ExplorationOutcome>(FIRST_EXP_PUN + ((mOutcome2 + 1) % (LAST_EXP_PUN + 1)));
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
