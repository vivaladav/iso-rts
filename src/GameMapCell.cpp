#include "GameMapCell.h"

#include "Unit.h"

namespace game
{

GameMapCell::~GameMapCell()
{
    delete obj;
}

Unit * GameMapCell::GetUnit() const
{
    if(obj != nullptr && obj->GetObjectType() == GameObjectType::OBJ_UNIT)
        return static_cast<Unit *>(obj);
    else
        return nullptr;
}

bool GameMapCell::HasUnit() const
{
    return obj != nullptr && obj->GetObjectType() == GameObjectType::OBJ_UNIT;
}

} // namespace game
