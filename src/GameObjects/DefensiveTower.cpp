#include "GameObjects/DefensiveTower.h"

#include "GameData.h"
#include "GameMap.h"
#include "IsoObject.h"
#include "Player.h"

#include <graphic/TextureManager.h>

namespace game
{

DefensiveTower::DefensiveTower(int rows, int cols)
    : GameObject(GameObjectType::OBJ_DEF_TOWER, rows, cols)
{
    SetStructure(true);

    SetImage();
}

void DefensiveTower::Update(float delta)
{
    CheckForEnemies();

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
                mTarget = nullptr;

            mTimerAttack = mTimeAttack;
        }
    }
}

void DefensiveTower::UpdateImage()
{
    SetImage();
}

void DefensiveTower::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const Player * owner = GetOwner();

    // avoid to set an image when there's no owner set
    if(nullptr == owner)
        return ;

    // set texture
    const unsigned int faction = owner->GetFaction();

    const int ind = SpriteIdStructures::ID_STRUCT_TOWER_L1_F1 + faction;

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, ind);
    isoObj->SetTexture(tex);
}

void DefensiveTower::CheckForEnemies()
{
    const GameMap * gm = GetGameMap();

    const int row = GetRow0();
    const int col = GetCol0();

    const int mapRows = gm->GetNumRows();
    const int mapCols = gm->GetNumCols();

    std::vector<GameObject *> objs;

    // find all enemies in range
    for(int i = 1; i <= mAttackRadius; ++i)
    {
        const int rTL = (row - i) > 0 ? (row - i) : 0;
        const int rBR = (row + i) < mapRows ? (row + i) : (mapRows - 1);

        const int cTL = (col - i) > 0 ?  (col - i) : 0;
        const int cBR = (col + i) < mapCols ? (row + i) : (mapRows - 1);

        for(int r = rTL; r <= rBR; ++r)
        {
            for(int c = cTL; c <= cBR; ++c)
            {
                const GameMapCell & cell = gm->GetCell(r, c);

                if(cell.obj && cell.obj->GetOwner() != GetOwner())
                    objs.push_back(cell.obj);
            }
        }
    }

    // no potential target
    if(objs.empty())
    {
        mTarget = nullptr;
        return ;
    }

    // chose target
    // TODO more complex logic, for now just selecting 1st one
    mTarget = objs.front();
}

void DefensiveTower::Shoot()
{
    // TEMP test
    mTarget->SumHealth(-mWeaponDamage);
}

} // namespace game
