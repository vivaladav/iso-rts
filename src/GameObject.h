#pragma once

namespace game
{

enum GameObjectType : unsigned int
{
    OBJ_RES_GEN,
    OBJ_RES_GEN_ENERGY,
    OBJ_RES_GEN_MATERIAL1,

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

    int GetRow0() const;
    int GetCol0() const;
    int GetRow1() const;
    int GetCol1() const;

    void SetSize(unsigned int rows, unsigned int cols);
    unsigned int GetRows() const;
    unsigned int GetCols() const;

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

    unsigned int mRows = 1;
    unsigned int mCols = 1;
};

inline const GameMapCell * GameObject::GetCell() const { return mCell; }

inline int GameObject::GetOwner() const { return mOwner; }

inline GameObjectType GameObject::GetObjectType() const { return mType; }

inline GameObjectImageId GameObject::GetImageId() const { return mImageId; }

inline void GameObject::SetSize(unsigned int rows, unsigned int cols)
{
    mRows = rows;
    mCols = cols;
}

inline unsigned int GameObject::GetRows() const { return mRows; }
inline unsigned int GameObject::GetCols() const { return mCols; }

} // namespace game
