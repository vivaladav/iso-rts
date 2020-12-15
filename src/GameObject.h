#pragma once

namespace game
{

enum GameObjectType : unsigned int
{
    OBJ_RES_GEN,
    OBJ_UNIT,

    NUM_GAME_OBJECT_TYPES,

    OBJ_NULL
};

enum GameObjectImageId : unsigned int;

class GameMapCell;

class GameObject
{
public:
    GameObject(GameObjectType type, int owner);
    virtual ~GameObject();

    const GameMapCell * GetCell() const;
    void SetCell(GameMapCell * cell);

    int GetOwner() const;
    void SetOwner(int owner);

    GameObjectType GetObjectType() const;

    GameObjectImageId GetImageId() const;

protected:
    virtual void UpdateImageId() = 0;

protected:
    GameObjectImageId mImageId;

private:
    GameMapCell * mCell = nullptr;

    int mOwner = -1;
    GameObjectType mType = OBJ_NULL;
};

inline const GameMapCell * GameObject::GetCell() const { return mCell; }

inline int GameObject::GetOwner() const { return mOwner; }

inline GameObjectType GameObject::GetObjectType() const { return mType; }

inline GameObjectImageId GameObject::GetImageId() const { return mImageId; }

} // namespace game
