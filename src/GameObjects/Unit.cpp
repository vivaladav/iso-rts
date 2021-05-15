#include "GameObjects/Unit.h"

#include "GameConstants.h"
#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"
#include "GameObjects/UnitData.h"
#include "Particles/UpdaterSingleLaser.h"
#include "Screens/ScreenGame.h"

#include <graphic/Texture.h>
#include <graphic/TextureManager.h>

#include <cmath>

namespace game
{

Unit::Unit(const UnitData & data, int rows, int cols)
    : GameObject(GameObjectType::OBJ_UNIT, rows, cols)
    , mUnitType(data.type)
{
    // SET STATS values in range [1-10]
    mStats.resize(NUM_UNIT_STATS);
    mStats[USTAT_ENERGY] = data.statEnergy;
    mStats[USTAT_RESISTANCE] = data.statResistance;
    mStats[USTAT_ATTACK] = data.statAttack;
    mStats[USTAT_SPEED] = data.statSpeed;
    mStats[USTAT_CONSTRUCTION] = data.statConstruction;
    mStats[USTAT_CONQUEST] = data.statConquest;

    // TODO translate stats into actual values, ex.: speed = 5 -> SetSpeed(2.f)

    // SET CONCRETE ATTRIBUTES
    const float maxStatVal = 10.f;

    // set actual speed
    const float maxSpeed = 5.f;
    const float speed = maxSpeed * static_cast<float>(mStats[USTAT_SPEED]) / maxStatVal;
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

void Unit::HandleOtherObjectDestroyed(GameObject * obj)
{
    if(mTarget == obj)
        mTarget = nullptr;
}

void Unit::Update(float delta)
{
    // attacking other object
    if(mTarget)
    {
        mTimerAttack -= delta;

        // time to shoot!
        if(mTimerAttack < 0.f)
        {
            Shoot();

            mTimerAttack = mTimeAttack;
        }
    }
}

void Unit::UpdateImage()
{
    SetImage();
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

    const float x0 = isoObj->GetX() + isoObj->GetWidth()* 0.5f;
    const float y0 = isoObj->GetY();
    const float tX = isoTarget->GetX() + (isoTarget->GetWidth() - tex->GetWidth()) * 0.5f;
    const float tY = isoTarget->GetY() + (isoTarget->GetHeight() - tex->GetHeight()) * 0.5f;
    const float speed = 400.f;

    const float rad2deg = 180.f / M_PI;
    const float dy0 = tY - y0;
    const float dx1 = tX - x0;
    const float dy1 = dy0;
    const float s = dy0 / sqrtf(dx1 * dx1 + dy1 * dy1);
    const float as = asinf(s);
    const double angle = as * rad2deg;

    const ParticleDataSingleLaser pd =
    {
        tex,
        angle,
        x0,
        y0,
        tX,
        tY,
        speed
    };

    pu->AddParticle(pd);

    // TODO assign damage after particle has hit, not before
    mTarget->SumHealth(-mWeaponDamage);
}

} // namespace game
