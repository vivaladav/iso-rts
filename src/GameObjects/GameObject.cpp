#include "GameObject.h"

#include "GameConstants.h"
#include "GameData.h"
#include "GameMapCell.h"
#include "IsoObject.h"
#include "Player.h"
#include "Particles/DataParticleDamage.h"
#include "Particles/UpdaterDamage.h"
#include "Screens/ScreenGame.h"

#include <sgl/core/Math.h>
#include <sgl/graphic/TextureManager.h>
#include <sgl/utilities/UniformDistribution.h>

namespace game
{

const unsigned int GameObject::COLOR_FOW = 0x555555FF;
const unsigned int GameObject::COLOR_VIS = 0xFFFFFFFF;

unsigned int GameObject::counter = 0;

// -- OBJECT TYPE --
const GameObjectTypeId GameObject::TYPE_NULL = 0;

const GameObjectTypeId GameObject::TYPE_BASE = std::hash<std::string>{}("BASE");
const GameObjectTypeId GameObject::TYPE_BLOBS = std::hash<std::string>{}("BLOBS");
const GameObjectTypeId GameObject::TYPE_DEFENSIVE_TOWER = std::hash<std::string>{}("DEF_TOWER");
const GameObjectTypeId GameObject::TYPE_DIAMONDS = std::hash<std::string>{}("DIAMONDS");
const GameObjectTypeId GameObject::TYPE_MOUNTAINS = std::hash<std::string>{}("MOUNTAINS");
const GameObjectTypeId GameObject::TYPE_PRACTICE_TARGET = std::hash<std::string>{}("TARGET");
const GameObjectTypeId GameObject::TYPE_RADAR_STATION = std::hash<std::string>{}("RADAR_STATION");
const GameObjectTypeId GameObject::TYPE_RADAR_TOWER = std::hash<std::string>{}("RADAR_TOWER");
const GameObjectTypeId GameObject::TYPE_RES_GEN_ENERGY = std::hash<std::string>{}("RESGEN_ENER");
const GameObjectTypeId GameObject::TYPE_RES_GEN_ENERGY_SOLAR = std::hash<std::string>{}("RESGEN_SOLAR");
const GameObjectTypeId GameObject::TYPE_RES_GEN_MATERIAL = std::hash<std::string>{}("RESGEN_MAT");
const GameObjectTypeId GameObject::TYPE_RES_GEN_MATERIAL_EXTRACT = std::hash<std::string>{}("RESGEN_MAT_EXT");
const GameObjectTypeId GameObject::TYPE_RES_STORAGE_BLOBS = std::hash<std::string>{}("RESSTOR_BLOBS");
const GameObjectTypeId GameObject::TYPE_RES_STORAGE_DIAMONDS = std::hash<std::string>{}("RESSTOR_DIAM");
const GameObjectTypeId GameObject::TYPE_RES_STORAGE_ENERGY = std::hash<std::string>{}("RESSTOR_ENER");
const GameObjectTypeId GameObject::TYPE_RES_STORAGE_MATERIAL = std::hash<std::string>{}("RESSTOR_MAT");
const GameObjectTypeId GameObject::TYPE_ROCKS = std::hash<std::string>{}("ROCKS");
const GameObjectTypeId GameObject::TYPE_TREES = std::hash<std::string>{}("TREES");
const GameObjectTypeId GameObject::TYPE_UNIT_SCOUT1 = std::hash<std::string>{}("UNIT_SCOUT1");
const GameObjectTypeId GameObject::TYPE_UNIT_SOLDIER1 = std::hash<std::string>{}("UNIT_SOLDIER1");
const GameObjectTypeId GameObject::TYPE_UNIT_SOLDIER2 = std::hash<std::string>{}("UNIT_SOLDIER2");
const GameObjectTypeId GameObject::TYPE_UNIT_WORKER1 = std::hash<std::string>{}("UNIT_WORKER1");
const GameObjectTypeId GameObject::TYPE_WALL = std::hash<std::string>{}("WALL");
const GameObjectTypeId GameObject::TYPE_WALL_GATE = std::hash<std::string>{}("WALL_GATE");

const std::unordered_map<GameObjectTypeId, std::string> GameObject::TITLES =
{
    { GameObject::TYPE_BASE, "BASE"},
    { GameObject::TYPE_DEFENSIVE_TOWER, "DEFENSIVE TOWER"},
    { GameObject::TYPE_MOUNTAINS, "MOUNTAINS"},
    { GameObject::TYPE_PRACTICE_TARGET, "PRACTICE TARGET"},
    { GameObject::TYPE_RADAR_STATION, "RADAR STATION"},
    { GameObject::TYPE_RADAR_TOWER, "RADAR TOWER"},
    { GameObject::TYPE_RES_GEN_ENERGY, "ENERGY GENERATOR"},
    { GameObject::TYPE_RES_GEN_ENERGY_SOLAR, "SOLAR PANELS"},
    { GameObject::TYPE_RES_GEN_MATERIAL, "MATERIAL GENERATOR"},
    { GameObject::TYPE_RES_GEN_MATERIAL_EXTRACT, "MATERIAL EXTRACTOR"},
    { GameObject::TYPE_RES_STORAGE_BLOBS, "BLOBS STORAGE"},
    { GameObject::TYPE_RES_STORAGE_DIAMONDS, "DIAMONDS STORAGE"},
    { GameObject::TYPE_RES_STORAGE_ENERGY, "ENERGY STORAGE"},
    { GameObject::TYPE_RES_STORAGE_MATERIAL, "MATERIAL STORAGE"},
    { GameObject::TYPE_ROCKS, "ROCKS"},
    { GameObject::TYPE_TREES, "TREES"},
    { GameObject::TYPE_UNIT_WORKER1, "WORKER"},
    { GameObject::TYPE_UNIT_SOLDIER1, "SOLDIER"},
    { GameObject::TYPE_UNIT_SOLDIER2, "SOLDIER"},
    { GameObject::TYPE_UNIT_SCOUT1, "SCOUT"},
    { GameObject::TYPE_WALL, "WALL"},
    { GameObject::TYPE_WALL_GATE, "GATE"}
};

const std::unordered_map<GameObjectTypeId, std::string> GameObject::DESCRIPTIONS =
{
    { GameObject::TYPE_BASE, "A control center. It can create units. "
                             "You need to defend it if you don't want to lose a territory."},
    { GameObject::TYPE_DEFENSIVE_TOWER, "A basic defensive tower."},
    { GameObject::TYPE_MOUNTAINS, "Some mountains"},
    { GameObject::TYPE_PRACTICE_TARGET, "A practice target.\nIt can be used to train your units "
                                        "and to improve their attack skills."},
    { GameObject::TYPE_RADAR_STATION, "A powerful radar that can show a big portion of the map."},
    { GameObject::TYPE_RADAR_TOWER, "A compact radar that can show a small portion of the map."},
    { GameObject::TYPE_RES_GEN_ENERGY, "A generator of energy.\nConnect it to your base to produce energy."},
    { GameObject::TYPE_RES_GEN_ENERGY_SOLAR, "A solar panel.\nConnect it to your base to produce energy."},
    { GameObject::TYPE_RES_GEN_MATERIAL, "A generator of material.\nConnect it to your base to mine material."},
    { GameObject::TYPE_RES_GEN_MATERIAL_EXTRACT, "A mine that can extract material from the ground.\n"
                                                 "Connect it to your base to produce material."},
    { GameObject::TYPE_RES_STORAGE_BLOBS, "Storage unit that can contain 100 units of blobs."},
    { GameObject::TYPE_RES_STORAGE_DIAMONDS, "Storage unit that can contain 150 units of diamonds."},
    { GameObject::TYPE_RES_STORAGE_ENERGY, "Storage unit that can contain 500 units of energy."},
    { GameObject::TYPE_RES_STORAGE_MATERIAL, "Storage unit that can contain 250 units of material."},
    { GameObject::TYPE_ROCKS, "Some rocks."},
    { GameObject::TYPE_TREES, "A single tree which will slowly grow into a forest."},
    { GameObject::TYPE_UNIT_WORKER1, "A basic worker unit.\nIt is specialized in construction and conquest."},
    { GameObject::TYPE_UNIT_SOLDIER1, "A basic soldier unit.\nUseful for defense and exploration."},
    { GameObject::TYPE_UNIT_SOLDIER2, "A slow, but versatile unit."},
    { GameObject::TYPE_UNIT_SCOUT1, "A light and fast unit ideal for exploring, but not for fighting."},
    { GameObject::TYPE_WALL, "A defensive wall."},
    { GameObject::TYPE_WALL_GATE, "A gate that can be controlled to open a passage through a defensive wall."}
};

// -- OBJECT CATEGORY --
const GameObjectCategoryId GameObject::CAT_NULL = 0;

const GameObjectCategoryId GameObject::CAT_COLLECTABLE = std::hash<std::string>{}("COLLECTABLE");
const GameObjectCategoryId GameObject::CAT_GENERIC = std::hash<std::string>{}("GENERIC");
const GameObjectCategoryId GameObject::CAT_RES_GENERATOR = std::hash<std::string>{}("RES_GEN");
const GameObjectCategoryId GameObject::CAT_RES_STORAGE = std::hash<std::string>{}("RES_STORAGE");
const GameObjectCategoryId GameObject::CAT_SCENE_OBJ = std::hash<std::string>{}("SCENE_OBJ");
const GameObjectCategoryId GameObject::CAT_UNIT = std::hash<std::string>{}("UNIT");

// -- CONSTRUCTOR & DESTRUCTOR --
GameObject::GameObject(GameObjectTypeId type, GameObjectCategoryId cat, int rows, int cols)
    : mOnValuesChanged([](){})
    , mIsoObj(new IsoObject(rows, cols))
    , mObjId(++counter)
    , mType(type)
    , mCategory(cat)
    , mRows(rows)
    , mCols(cols)
{
    // default colors to mark objects that haven't set any
    mObjColors.push_back(0xFFFFFFFF);
    mObjColors.push_back(0xFF00FFFF);
}

GameObject::~GameObject() { delete mIsoObj; }

void GameObject::SetSelected(bool val)
{
    // same value -> nothing to do
    if(val == mSelected)
        return ;

    mSelected = val;

    UpdateGraphics();
}

void GameObject::SetVisible(bool val)
{
    mVisible = val;

    UpdateGraphics();
}

void GameObject::SetLinked(bool val)
{
    if(val == mLinked)
        return ;

    mLinked = val;

    OnLinkedChanged();
}

void GameObject::SetCell(const GameMapCell * cell)
{
    mCell = cell;

    mIsoObj->SetRow(cell->row);
    mIsoObj->SetCol(cell->col);
}

int GameObject::GetRow0() const { return mCell->row; }
int GameObject::GetCol0() const { return mCell->col; }
int GameObject::GetRow1() const { return 1 + mCell->row - mRows; }
int GameObject::GetCol1() const { return 1 + mCell->col - mCols; }

void GameObject::SetOwner(Player * owner)
{
    // setting same owner again -> nothing to do
    if(owner == mOwner)
        return ;

    mOwner = owner;

    UpdateGraphics();
}

void GameObject::SetHealth(float val)
{
    const float oldH = mHealth;

    mHealth = val;

    if(mHealth > mMaxHealth)
        mHealth = mMaxHealth;
    else if(mHealth < 0.f)
        mHealth = 0.f;

    const float minDelta = 0.01f;
    const float diff = std::fabs(mHealth - oldH);

    if(diff > minDelta)
        mOnValuesChanged();
}

void GameObject::SumHealth(float val)
{
    SetHealth(mHealth + val);
}

void GameObject::SetEnergy(float val)
{
    const float oldEn = mEnergy;

    mEnergy = val;

    if(mEnergy > mMaxEnergy)
        mEnergy = mMaxEnergy;
    else if(mEnergy < 0.f)
        mEnergy = 0.f;

    const float minDelta = 0.01f;
    const float diff = std::fabs(mEnergy - oldEn);

    if(diff > minDelta)
        mOnValuesChanged();
}

void GameObject::SumEnergy(float val)
{
    SetEnergy(mEnergy + val);
}

void GameObject::Hit(float damage)
{
    using namespace sgl::graphic;

    SumHealth(-damage);

    const int numPart0 = 20 * mRows * mCols;
    int numPart = numPart0;

    const int maxQuad = 4;
    int numQuad = 1;

    const float angInc = 90.f;
    float ang0 = 0.f;

    // hit, but alive -> pick random quadrant
    if(mHealth > 0.f)
    {
        const int quad0 = 0;
        sgl::utilities::UniformDistribution genQuad(quad0, maxQuad - 1);

        ang0 += angInc * genQuad.GetNextValue();
    }
    // hit and destroyed -> use all quadrants
    else
    {
        const int multPart = maxQuad;
        numPart *= multPart;

        numQuad = maxQuad;
    }

    float ang1 = ang0 + angInc;

    const int numPartQuad = numPart / numQuad;

    auto pu = static_cast<UpdaterDamage *>(GetScreen()->GetParticleUpdater(PU_DAMAGE));

    const unsigned int texInd = SpriteIdParticles::ID_PART_RECT_4x4;
    Texture * tex = TextureManager::Instance()->GetSprite(SpriteFileParticles, texInd);

    IsoObject * isoObj = GetIsoObject();
    const float objXC = isoObj->GetX() + isoObj->GetWidth() * 0.5f;
    const float objYC = isoObj->GetY() + isoObj->GetHeight() * 0.5f;

    // random generator of rotation angle
    const int minRot = 0;
    const int maxRot = 360;
    sgl::utilities::UniformDistribution genRot(minRot, maxRot);

    // random generator for velocity direction
    sgl::utilities::UniformDistribution genVel(static_cast<int>(ang0), static_cast<int>(ang1));

    const float deg2rad = sgl::core::Math::PIf / 180.f;

    // random generator for speed
    const int minSpeed = 100;
    const int maxSpeed = 300;

    sgl::utilities::UniformDistribution genSpeed(minSpeed, maxSpeed);

    // random generator for decay speed
    const int minDecSpeed = 250;
    const int maxDecSpeed = 500;
    sgl::utilities::UniformDistribution genDecSpeed(minDecSpeed, maxDecSpeed);

    // random generator for scale
    const int minScale = 1;
    const int maxScale = 2;
    sgl::utilities::UniformDistribution genScale(minScale, maxScale);

    // random generator for color
    const int color0 = 0;
    const int colorN = mObjColors.size() - 1;

    sgl::utilities::UniformDistribution genColor(color0, colorN);

    for(int q = 0; q < numQuad; ++q)
    {
        for(int p = 0; p < numPartQuad; ++p)
        {
            const double rot = genRot.GetNextValue();

            const float ang = genVel.GetNextValue() * deg2rad;
            const float velX = cosf(ang);
            const float velY = sinf(ang);

            const float speed = genSpeed.GetNextValue();

            const float decSpeed = genDecSpeed.GetNextValue();

            const unsigned int color = mObjColors[genColor.GetNextValue()];

            const float scale = 1.f / static_cast<float>(genScale.GetNextValue());

            DataParticleDamage data(tex, rot, objXC, objYC, velX, velY, speed, decSpeed, scale, color);

            pu->AddParticle(data);
        }

        ang0 += angInc;
        ang1 += angInc;

        genVel.SetParameters(static_cast<int>(ang0), static_cast<int>(ang1));
    }
}

void GameObject::SetActiveActionToDefault() { mActiveAction = IDLE; }

void GameObject::Update(float) { }

void GameObject::OnLinkedChanged() { }

void GameObject::SetDefaultColors()
{
    // clear current colors
    mObjColors.clear();

    // assign new colors based on faction
    Player * p = GetOwner();
    const PlayerFaction faction = p != nullptr ? p->GetFaction() : NO_FACTION;

    switch(faction)
    {
        case FACTION_1:
            mObjColors.push_back(0xd9938cff);
            mObjColors.push_back(0xcc6f66ff);
            mObjColors.push_back(0xc04a3fff);
            mObjColors.push_back(0xcc4133ff);
            mObjColors.push_back(0x9a3b32ff);
            mObjColors.push_back(0x86332cff);
        break;

        case FACTION_2:
            mObjColors.push_back(0x8cd992ff);
            mObjColors.push_back(0x66cc6eff);
            mObjColors.push_back(0x3fc04aff);
            mObjColors.push_back(0x33cc40ff);
            mObjColors.push_back(0x329a3bff);
            mObjColors.push_back(0x2c8633ff);
        break;

        case FACTION_3:
            mObjColors.push_back(0x8cccd9ff);
            mObjColors.push_back(0x66bbccff);
            mObjColors.push_back(0x3faac0ff);
            mObjColors.push_back(0x33b2ccff);
            mObjColors.push_back(0x32899aff);
            mObjColors.push_back(0x2c7786ff);
        break;

        // NO_FACTION
        default:
            mObjColors.push_back(0xccccccff);
            mObjColors.push_back(0xb2b2b2ff);
            mObjColors.push_back(0x999999ff);
            mObjColors.push_back(0x808080ff);
            mObjColors.push_back(0x666666ff);
            mObjColors.push_back(0x595959ff);
        break;
    }
}

} // namespace game
