#include "GameObjects/RadarStation.h"

#include "GameData.h"
#include "IsoObject.h"
#include "Player.h"

#include <sgl/graphic/TextureManager.h>

namespace game
{

RadarStation::RadarStation(int rows, int cols)
    : Structure(OBJ_RADAR_STATION, rows, cols)
{
    SetCanBeConquered(true);

    SetImage();

    SetVisibilityLevel(1);
}

void RadarStation::UpdateGraphics()
{
    SetImage();

    SetDefaultColors();
}

void RadarStation::OnLinkedChanged()
{
    GameObject::OnLinkedChanged();

    const int maxVis = 12;
    SetVisibilityLevel(maxVis * static_cast<int>(IsLinked()));
}

void RadarStation::SetImage()
{
    auto * tm = lib::graphic::TextureManager::Instance();

    IsoObject * isoObj = GetIsoObject();

    if(IsVisible())
        isoObj->SetColor(COLOR_VIS);
    else
        isoObj->SetColor(COLOR_FOW);

    const Player * owner = GetOwner();

    unsigned int texInd;

    if(nullptr == owner)
        texInd = ID_STRUCT_RADAR;
    else
    {
        const unsigned int faction = owner->GetFaction();

        texInd = SpriteIdStructures::ID_STRUCT_RADAR_F1 +
                 (faction * NUM_RADAR_SPRITES_PER_FAC) +
                 static_cast<int>(IsSelected());
    }

    lib::graphic::Texture * tex = tm->GetSprite(SpriteFileStructures, texInd);
    isoObj->SetTexture(tex);
}

} // namespace game

