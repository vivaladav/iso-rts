#include "GameObject.h"

#include "GameData.h"
#include "GameMapCell.h"
#include "IsoObject.h"
#include "Particles/DataParticleDamage.h"
#include "Particles/UpdaterDamage.h"
#include "Screens/ScreenGame.h"

#include <graphic/TextureManager.h>
#include <utilities/UniformDistribution.h>

#include <cmath>

namespace game
{

const unsigned int GameObject::COLOR_FOW = 0x555555FF;
const unsigned int GameObject::COLOR_VIS = 0xFFFFFFFF;

unsigned int GameObject::counter = 0;

GameObject::GameObject(GameObjectType type, int rows, int cols)
    : mIsoObj(new IsoObject(rows, cols))
    , mObjId(++counter)
    , mType(type)
{
}

GameObject::~GameObject() { delete mIsoObj; }

void GameObject::SetSelected(bool val)
{
    // same value -> nothing to do
    if(val == mSelected)
        return ;

    mSelected = val;

    UpdateImage();
}

void GameObject::SetVisible(bool val)
{
    mVisible = val;

    UpdateImage();
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

    UpdateImage();
}

void GameObject::Hit(float damage)
{
    using namespace lib::graphic;

    SumHealth(-damage);

    const int numPart0 = 15 * mRows * mCols;
    int numPart = numPart0;

    const int maxQuad = 4;
    int numQuad = 1;

    const float angInc = 90.f;
    float ang0 = 0.f;

    // hit, but alive -> pick random quadrant
    if(mHealth > 0.f)
    {
        const int quad0 = 0;
        lib::utilities::UniformDistribution genQuad(quad0, maxQuad - 1);

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
    lib::utilities::UniformDistribution genRot(minRot, maxRot);

    // random generator for velocity direction
    lib::utilities::UniformDistribution genVel(static_cast<int>(ang0), static_cast<int>(ang1));

    const float deg2rad = M_PI / 180.f;

    // random generator for speed
    const int minSpeed = 100;
    const int maxSpeed = 300;

    lib::utilities::UniformDistribution genSpeed(minSpeed, maxSpeed);

    // random generator for decay speed

    const int minDecSpeed = 250;
    const int maxDecSpeed = 500;

    lib::utilities::UniformDistribution genDecSpeed(minDecSpeed, maxDecSpeed);

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

            DataParticleDamage data(tex, rot, objXC, objYC, velX, velY, speed, decSpeed);

            pu->AddParticle(data);
        }

        ang0 += angInc;
        ang1 += angInc;

        genVel.SetParameters(static_cast<int>(ang0), static_cast<int>(ang1));
    }
}

void GameObject::Update(float) { }

void GameObject::OnLinkedChanged() { }

} // namespace game
