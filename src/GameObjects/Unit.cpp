#include "GameObjects/Unit.h"

#include "GameConstants.h"
#include "GameData.h"
#include "GameMap.h"
#include "IsoObject.h"
#include "GameObjects/ObjectData.h"
#include "Particles/DataParticleSingleLaser.h"
#include "Particles/UpdaterSingleLaser.h"
#include "Screens/ScreenGame.h"

#include <sgl/core/Math.h>
#include <sgl/graphic/Texture.h>
#include <sgl/graphic/TextureManager.h>

#include <unordered_map>
#include <cmath>

namespace game
{

const float ACTION_COSTS[NUM_OBJ_ACTIONS] =
{
    0.f,    // IDLE
    0.f,    // BUILD_UNIT
    2.f,    // MOVE
    5.f,    // CONQUER_CELL
    10.f,   // CONQUER_STRUCTURE
    1.f,    // ATTACK
    20.f,   // BUILD_STRUCTURE
    10.f,   // BUILD_WALL
    0.f     // TOGGLE_GATE
};

Unit::Unit(const ObjectBasicData & objData, const ObjectFactionData & facData)
    : GameObject(objData.objType, GameObject::CAT_UNIT, objData.rows, objData.cols)
    , mStructToBuild(GameObject::TYPE_NULL)
{
    // SET STATS values in range [1-10]
    mStats.resize(NUM_UNIT_STATS);
    mStats = facData.stats;

    // set attack range converting attribute
    const int maxAttVal = 11;
    const int attRanges[maxAttVal] = { 0, 2, 3, 4, 5, 6, 8, 9, 10, 11, 13 };
    mAttackRange = attRanges[mStats[OSTAT_FIRE_RANGE]];

    // TODO translate stats into actual values, ex.: speed = 5 -> SetSpeed(2.f)

    // SET CONCRETE ATTRIBUTES
    const float maxStatVal = 10.f;

    // set actual speed
    const float maxSpeed = 5.f;
    const float speed = maxSpeed * static_cast<float>(mStats[OSTAT_SPEED]) / maxStatVal;
    SetSpeed(speed);

    SetVisibilityLevel(4);
}

void Unit::IncreaseUnitLevel()
{
    if(mLevel >= MAX_UNITS_LEVEL)
        return ;

    ++mLevel;
    SetImage();
}

bool Unit::IsTargetInRange(GameObject * obj) const
{
    for(int r = obj->GetRow1(); r <= obj->GetRow0(); ++r)
    {
        for(int c = obj->GetCol1(); c <= obj->GetCol0(); ++c)
        {
            if(std::abs(GetRow0() - r) <= mAttackRange && std::abs(GetCol0() - c) <= mAttackRange)
                return true;
        }
    }

    return false;
}

bool Unit::SetAttackTarget(GameObject * obj)
{
   if(nullptr == obj || !IsTargetInRange(obj) || !obj->IsVisible() || obj == this)
       return false;

    mTarget = obj;

    return true;
}

void Unit::SetActiveActionToDefault() { SetActiveAction(MOVE); }

void Unit::Update(float delta)
{
    // UPDATE ENERGY
    // TODO recover energy based on attributes
    if(GetEnergy() < GetMaxEnergy())
        SumEnergy(0.1f);

    // ATTACKING OTHER OBJECTS
    if(mTarget)
    {
        mTimerAttack -= delta;

        // time to shoot!
        if(mTimerAttack < 0.f)
        {
            // target still alive -> try to shoot
            if(GetGameMap()->HasObject(mTarget))
            {
                if(IsTargetInRange(mTarget))
                    Shoot();
                else
                {
                    mTarget = nullptr;

                    // mark attack action as failed
                    GetScreen()->SetObjectActionFailed(this);
                    SetCurrentAction(GameObjectActionType::IDLE);
                }
            }
            // target destroyed -> stop
            else
            {
                mTarget = nullptr;

                // mark attack action as completed
                GetScreen()->SetObjectActionCompleted(this);
                SetCurrentAction(GameObjectActionType::IDLE);
            }

            mTimerAttack = mTimeAttack;
        }
    }
}

void Unit::ClearStructureToBuild() { mStructToBuild = GameObject::TYPE_NULL; }

void Unit::ConsumeEnergy(GameObjectActionType action)
{
    if(action < NUM_OBJ_ACTIONS)
       SumEnergy(-ACTION_COSTS[action]);
}

int Unit::GetStat(unsigned int index) const
{
    if(index < mStats.size())
        return mStats[index];
    else
        return 0;
}

bool Unit::HasEnergyForAction(GameObjectActionType action)
{
    if(action < NUM_OBJ_ACTIONS)
    {
        const float diff = GetEnergy() - ACTION_COSTS[action];
        return diff >= 0.f;
    }
    else
        return false;
}

void Unit::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void Unit::SetImage()
{
    const PlayerFaction faction = GetFaction();

    // avoid to set an image when there's no owner set
    if(NO_FACTION == faction)
        return ;

    const GameObjectTypeId type = GetObjectType();
    const unsigned int ind = TypeToIndex(type);

    const unsigned int texInd = (NUM_UNIT_SPRITES_PER_FACTION * faction) +
                                (NUM_UNIT_SPRITES_PER_TYPE * ind) +
                                 static_cast<unsigned int>(IsSelected());
    auto * tm = sgl::graphic::TextureManager::Instance();
    sgl::graphic::Texture * tex =tm->GetSprite(SpriteFileUnits, texInd);

    GetIsoObject()->SetTexture(tex);
}

void Unit::Shoot()
{
    using namespace sgl::graphic;
    // TODO calculate chance of hitting based on attack and defense attributes
    // for now assuming it's always hit

    const PlayerFaction faction = GetFaction();

    // avoid to set an image when there's no owner set
    if(NO_FACTION == faction)
        return ;

    // consume energy
    ConsumeEnergy(ATTACK);

    auto pu = static_cast<UpdaterSingleLaser *>(GetScreen()->GetParticleUpdater(PU_SINGLE_LASER));

    const unsigned int texInd = SpriteIdUnitsParticles::SPR_UPART_LASER_F1 + faction;
    Texture * tex = TextureManager::Instance()->GetSprite(SpriteFileUnitsParticles, texInd);

    IsoObject * isoObj = GetIsoObject();
    IsoObject * isoTarget = mTarget->GetIsoObject();

    const float x0 = isoObj->GetX() + isoObj->GetWidth() * 0.5f;
    const float y0 = isoObj->GetY();
    const float tX = isoTarget->GetX() + (isoTarget->GetWidth() - tex->GetWidth()) * 0.5f;
    const float tY = isoTarget->GetY() + (isoTarget->GetHeight() - tex->GetHeight()) * 0.5f;
    const float speed = 400.f;

    const float rad2deg = 180.f / sgl::core::Math::PIf;
    const float dy0 = tY - y0;
    const float dx1 = tX - x0;
    const float dy1 = dy0;
    const float s = dy0 / sqrtf(dx1 * dx1 + dy1 * dy1);
    const float as = asinf(s);
    const double angleDeg = as * rad2deg;
    double angle;

    if(dx1 < 0.f)
    {
        // bottom left
        if(dy1 > 0.f)
            angle = 180.f - angleDeg;
        // top left
        else
            angle = 180.f - angleDeg;
    }
    else
    {
        // bottom right
        if(dy1 > 0.f)
            angle = angleDeg;
        // top right
        else
            angle = 360.f + angleDeg;
    }

    const DataParticleSingleLaser pd =
    {
        tex,
        GetGameMap(),
        mTarget,
        angle,
        x0,
        y0,
        tX,
        tY,
        speed,
        mWeaponDamage,
        GetFaction()
    };

    pu->AddParticle(pd);
}

unsigned int Unit::TypeToIndex(GameObjectTypeId type)
{
    const std::unordered_map<GameObjectTypeId, unsigned int> indexes =
    {
        {GameObject::TYPE_UNIT_WORKER1, 0},
        {GameObject::TYPE_UNIT_SOLDIER1, 1},
        {GameObject::TYPE_UNIT_SOLDIER2, 2},
        {GameObject::TYPE_UNIT_SCOUT1, 3}
    };

    return indexes.at(type);
}

GameObjectTypeId Unit::IndexToType(unsigned int ind)
{
    const GameObjectTypeId types[] =
    {
        GameObject::TYPE_UNIT_WORKER1,
        GameObject::TYPE_UNIT_SOLDIER1,
        GameObject::TYPE_UNIT_SOLDIER2,
        GameObject::TYPE_UNIT_SCOUT1
    };

    return types[ind];
}

} // namespace game
