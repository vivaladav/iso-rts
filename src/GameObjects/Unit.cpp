#include "GameObjects/Unit.h"

#include "GameConstants.h"
#include "GameData.h"
#include "GameMap.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/ObjectData.h"
#include "GameObjects/Structure.h"
#include "Particles/DataParticleSingleLaser.h"
#include "Particles/UpdaterSingleLaser.h"
#include "Screens/ScreenGame.h"

#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

#include <cmath>

namespace game
{

Unit::Unit(const ObjectData & data, int rows, int cols)
    : GameObject(GameObjectType::OBJ_UNIT, rows, cols)
    , mUnitType(static_cast<UnitType>(data.objType))
    , mStructToBuild(STRUCT_NULL)
{
    // SET STATS values in range [1-10]
    mStats.resize(NUM_UNIT_STATS);
    mStats = data.stats;

    // TODO translate stats into actual values, ex.: speed = 5 -> SetSpeed(2.f)

    // SET CONCRETE ATTRIBUTES
    const float maxStatVal = 10.f;

    // set actual speed
    const float maxSpeed = 10.f;
    const float speed = maxSpeed * static_cast<float>(mStats[OSTAT_SPEED]) / maxStatVal;
    SetSpeed(speed);

    SetVisibilityLevel(1);
}

void Unit::IncreaseUnitLevel()
{
    if(mLevel >= MAX_UNITS_LEVEL)
        return ;

    ++mLevel;
    SetImage();
}

void Unit::SetActiveActionToDefault() { SetActiveAction(MOVE); }

void Unit::Update(float delta)
{
    // attacking other object
    if(mTarget)
    {
        mTimerAttack -= delta;

        // time to shoot!
        if(mTimerAttack < 0.f)
        {
            // target still alive -> shoot
            if(GetGameMap()->HasObject(mTarget))
                Shoot();
            // target destroyed -> clear pointer
            else
            {
                mTarget = nullptr;

                // mark attack action as completed
                GetScreen()->SetObjectActionCompleted(this);
                SetCurrentAction(GameObjectActionId::IDLE);
            }

            mTimerAttack = mTimeAttack;
        }
    }
}

void Unit::ClearStructureToBuild() { mStructToBuild = STRUCT_NULL; }

void Unit::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void Unit::SetImage()
{

    const Player * owner = GetOwner();

    // avoid to set an image when there's no owner set
    if(nullptr == owner)
        return ;

    const unsigned int faction = owner->GetFaction();

    const unsigned int texInd = (NUM_UNIT_SPRITES_PER_FACTION * faction) +
                                (NUM_UNIT_SPRITES_PER_TYPE * mUnitType) +
                                 static_cast<unsigned int>(IsSelected());

    auto * tm = lib::graphic::TextureManager::Instance();
    lib::graphic::Texture * tex =tm->GetSprite(SpriteFileUnits, texInd);

    GetIsoObject()->SetTexture(tex);
}

void Unit::Shoot()
{
    using namespace lib::graphic;
    // TODO calculate chance of hitting based on attack and defense attributes
    // for now assuming it's always hit

    const Player * owner = GetOwner();

    // avoid to set an image when there's no owner set
    if(nullptr == owner)
        return ;

    auto pu = static_cast<UpdaterSingleLaser *>(GetScreen()->GetParticleUpdater(PU_SINGLE_LASER));

    const unsigned int texInd = SpriteIdUnitsParticles::SPR_UPART_LASER_F1 + owner->GetFaction();
    Texture * tex = TextureManager::Instance()->GetSprite(SpriteFileUnitsParticles, texInd);

    IsoObject * isoObj = GetIsoObject();
    IsoObject * isoTarget = mTarget->GetIsoObject();

    const float x0 = isoObj->GetX() + isoObj->GetWidth() * 0.5f;
    const float y0 = isoObj->GetY();
    const float tX = isoTarget->GetX() + (isoTarget->GetWidth() - tex->GetWidth()) * 0.5f;
    const float tY = isoTarget->GetY() + (isoTarget->GetHeight() - tex->GetHeight()) * 0.5f;
    const float speed = 300.f;

    const float rad2deg = 180.f / M_PI;
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
        mWeaponDamage
    };

    pu->AddParticle(pd);
}

} // namespace game
