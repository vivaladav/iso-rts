#pragma once

namespace game
{

enum GameObjectType : unsigned int
{
    OBJ_BASE,
    OBJ_BASE_F1,
    OBJ_BASE_F2,
    OBJ_BASE_F3,

    OBJ_RES_GEN,
    OBJ_RES_GEN_ENERGY,
    OBJ_RES_GEN_MATERIAL1,

    OBJ_UNIT,

    NUM_GAME_OBJECT_TYPES,

    OBJ_NULL
};

class GameMapCell;
class IsoObject;

class GameObject
{
public:
    GameObject(GameObjectType type, int owner, int rows, int cols);
    virtual ~GameObject();

    IsoObject * GetIsoObject() const;

    bool IsSelected() const;
    void SetSelected(bool val);

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

protected:
    virtual void UpdateImage() = 0;

protected:
    IsoObject * mIsoObj = nullptr;

private:
    GameMapCell * mCell = nullptr;

    int mOwner = -1;
    GameObjectType mType = OBJ_NULL;

    unsigned int mRows = 1;
    unsigned int mCols = 1;

    bool mSelected = false;
};

inline IsoObject * GameObject::GetIsoObject() const { return mIsoObj; }

inline bool GameObject::IsSelected() const { return mSelected; }

inline const GameMapCell * GameObject::GetCell() const { return mCell; }

inline int GameObject::GetOwner() const { return mOwner; }

inline GameObjectType GameObject::GetObjectType() const { return mType; }

inline void GameObject::SetSize(unsigned int rows, unsigned int cols)
{
    mRows = rows;
    mCols = cols;
}

inline unsigned int GameObject::GetRows() const { return mRows; }
inline unsigned int GameObject::GetCols() const { return mCols; }

} // namespace game
