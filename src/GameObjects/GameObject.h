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

    OBJ_SCENE_OBJECT,
    OBJ_MOUNTAIN_ROW_END_L_1,
    OBJ_MOUNTAIN_ROW_END_R_1,
    OBJ_MOUNTAIN_COL_END_L_1,
    OBJ_MOUNTAIN_COL_END_R_1,
    OBJ_MOUNTAIN_ROW_1,
    OBJ_MOUNTAIN_ROW_2,
    OBJ_MOUNTAIN_ROW_3,
    OBJ_MOUNTAIN_COL_1,
    OBJ_MOUNTAIN_COL_2,
    OBJ_MOUNTAIN_COL_3,
    OBJ_MOUNTAIN_CORN_BL_1,
    OBJ_MOUNTAIN_CORN_BR_1,
    OBJ_MOUNTAIN_CORN_TL_1,
    OBJ_MOUNTAIN_CORN_TR_1,
    OBJ_MOUNTAIN_FIRST = OBJ_MOUNTAIN_ROW_END_L_1,
    OBJ_MOUNTAIN_LAST = OBJ_MOUNTAIN_CORN_TR_1,

    OBJ_DIAMONDS,

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

    unsigned int GetObjectId() const;

    IsoObject * GetIsoObject() const;

    bool IsStructure() const;
    bool CanBeConquered() const;

    bool IsSelected() const;
    void SetSelected(bool val);

    bool IsBusy() const;
    void SetBusy(bool val);

    bool IsVisited() const;
    void SetVisited();

    bool IsVisible() const;
    void SetVisible(bool val);

    const GameMapCell * GetCell() const;
    void SetCell(const GameMapCell * cell);

    int GetVisibilityLevel() const;

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

    float GetSpeed() const;

    virtual void Update(float delta);

protected:
    virtual void UpdateImage() = 0;

    void SetVisibilityLevel(int val);
    void SetStructure(bool val);
    void SetCanBeConquered(bool val);

    void SetSpeed(float speed);

protected:
    static const unsigned int COLOR_FOW;
    static const unsigned int COLOR_VIS;

private:
    static unsigned int counter;

private:
    IsoObject * mIsoObj = nullptr;

    unsigned int mObjId;

    const GameMapCell * mCell = nullptr;

    int mOwner = -1;
    GameObjectType mType = OBJ_NULL;

    unsigned int mRows = 1;
    unsigned int mCols = 1;

    int mVisLevel = 0;

    float mSpeed = 0.f;

    bool mStructure = false;

    bool mCanBeConq = false;

    bool mSelected = false;

    bool mBusy = false;

    bool mVisible = true;
    bool mVisited = false;
};

inline unsigned int GameObject::GetObjectId() const { return mObjId; }

inline IsoObject * GameObject::GetIsoObject() const { return mIsoObj; }

inline bool GameObject::IsStructure() const { return mStructure; }

inline bool GameObject::CanBeConquered() const { return mCanBeConq; }

inline bool GameObject::IsSelected() const { return mSelected; }

inline bool GameObject::IsBusy() const { return mBusy; }
inline void GameObject::SetBusy(bool val) { mBusy = val; }

inline bool GameObject::IsVisited() const { return mVisited; }
inline void GameObject::SetVisited() { mVisited = true; }

inline bool GameObject::IsVisible() const { return mVisible; }

inline const GameMapCell * GameObject::GetCell() const { return mCell; }

inline int GameObject::GetVisibilityLevel() const { return mVisLevel; }

inline int GameObject::GetOwner() const { return mOwner; }

inline GameObjectType GameObject::GetObjectType() const { return mType; }

inline void GameObject::SetSize(unsigned int rows, unsigned int cols)
{
    mRows = rows;
    mCols = cols;
}

inline unsigned int GameObject::GetRows() const { return mRows; }
inline unsigned int GameObject::GetCols() const { return mCols; }

inline void GameObject::SetVisibilityLevel(int val) { mVisLevel = val; }
inline void GameObject::SetStructure(bool val) { mStructure = val; }
inline void GameObject::SetCanBeConquered(bool val) { mCanBeConq = val; }

inline float GameObject::GetSpeed() const { return mSpeed; }
inline void GameObject::SetSpeed(float speed) { mSpeed = speed; }

} // namespace game
