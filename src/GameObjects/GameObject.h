#pragma once

namespace game
{

enum class MapObjectId : unsigned int
{
    // BASE START POINT
    BASE_P1 = 1000,
    BASE_P2,
    BASE_P3,

    // ENERGY SOURCES
    GEN_ENERGY = 2000,
    GEN_MATERIAL1,

    // COLLECTABLES
    DIAMONDS = 3000,

    // SCENE OBJECTS
    MOUNTAIN_ROW_END_L_1 = 4000,
    MOUNTAIN_ROW_END_R_1,
    MOUNTAIN_COL_END_L_1,
    MOUNTAIN_COL_END_R_1,
    MOUNTAIN_ROW_1,
    MOUNTAIN_ROW_2,
    MOUNTAIN_ROW_3,
    MOUNTAIN_COL_1,
    MOUNTAIN_COL_2,
    MOUNTAIN_COL_3,
    MOUNTAIN_CORN_BL_1,
    MOUNTAIN_CORN_BR_1,
    MOUNTAIN_CORN_TL_1,
    MOUNTAIN_CORN_TR_1,
    MOUNTAIN_INTN,
    MOUNTAIN_INTS,
    MOUNTAIN_INTW,
    MOUNTAIN_INTE,
    MOUNTAIN_CROSS,

    PRACTICE_TARGET = 4019,    // 4019

    // UNITS
    UNIT = 5000,

    // RANGE LIMITS
    MOUNTAIN_FIRST = MOUNTAIN_ROW_END_L_1,
    MOUNTAIN_LAST = MOUNTAIN_CROSS,

    // SPECIAL IDs
    OBJ_NULL
};

enum GameObjectType : unsigned int
{
    OBJ_BASE,

    OBJ_RES_GEN,
    OBJ_RES_GEN_ENERGY,
    OBJ_RES_GEN_MATERIAL1,

    OBJ_UNIT,

    OBJ_SCENE_OBJECT,
    OBJ_ROCKS_ROW_END_L_1,
    OBJ_ROCKS_ROW_END_R_1,
    OBJ_ROCKS_COL_END_L_1,
    OBJ_ROCKS_COL_END_R_1,
    OBJ_ROCKS_ROW_1,
    OBJ_ROCKS_ROW_2,
    OBJ_ROCKS_ROW_3,
    OBJ_ROCKS_COL_1,
    OBJ_ROCKS_COL_2,
    OBJ_ROCKS_COL_3,
    OBJ_ROCKS_CORN_BL_1,
    OBJ_ROCKS_CORN_BR_1,
    OBJ_ROCKS_CORN_TL_1,
    OBJ_ROCKS_CORN_TR_1,
    OBJ_ROCKS_INTN,
    OBJ_ROCKS_INTS,
    OBJ_ROCKS_INTW,
    OBJ_ROCKS_INTE,
    OBJ_ROCKS_CROSS,

    OBJ_BLOBS,
    OBJ_DIAMONDS,

    OBJ_WALL,
    OBJ_WALL_HORIZ,
    OBJ_WALL_VERT,
    OBJ_WALL_TL,
    OBJ_WALL_TR,
    OBJ_WALL_BL,
    OBJ_WALL_BR,
    OBJ_WALL_INTN,
    OBJ_WALL_INTS,
    OBJ_WALL_INTW,
    OBJ_WALL_INTE,
    OBJ_WALL_CROSS,

    OBJ_PRACTICE_TARGET,

    NUM_GAME_OBJECT_TYPES,

    // RANGE LIMITS
    OBJ_ROCKS_FIRST = OBJ_ROCKS_ROW_END_L_1,
    OBJ_ROCKS_LAST = OBJ_ROCKS_CROSS,
    OBJ_WALL_FIRST = OBJ_WALL_HORIZ,
    OBJ_WALL_LAST = OBJ_WALL_CROSS,

    // RANGE COUNTERS
    NUM_OBJS_WALL = OBJ_WALL_LAST - OBJ_WALL_FIRST + 1,

    // SPECIAL IDs
    OBJ_NULL
};

enum GameObjectActionId : unsigned int
{
    IDLE,

    // BASE
    BUILD_UNIT,

    // UNIT
    MOVE,
    CONQUER_CELL,
    CONQUER_STRUCTURE,
    ATTACK,
    BUILD_WALL,

    NUM_OBJ_ACTIONS
};

class GameMap;
class GameMapCell;
class IsoObject;
class Player;
class ScreenGame;

class GameObject
{
public:
    GameObject(GameObjectType type, int rows, int cols);
    virtual ~GameObject();

    void SetGameMap(GameMap * map);
    void SetScreen(ScreenGame * screen);

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

    bool IsDestroyed() const;

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

    Player * GetOwner() const;
    void SetOwner(Player * owner);

    GameObjectType GetObjectType() const;

    float GetHealth() const;
    void SumHealth(float val);

    float GetSpeed() const;

    GameObjectActionId GetActiveAction() const;
    void SetActiveAction(GameObjectActionId action);

    virtual void Update(float delta);

protected:
    virtual void UpdateImage() = 0;

    GameMap * GetGameMap() const;
    ScreenGame * GetScreen() const;

    void SetVisibilityLevel(int val);
    void SetStructure(bool val);
    void SetCanBeConquered(bool val);

    void SetHealth(float health);
    void SetMaxHealth(float max);
    void SetSpeed(float speed);

protected:
    static const unsigned int COLOR_FOW;
    static const unsigned int COLOR_VIS;

private:
    static unsigned int counter;

private:
    IsoObject * mIsoObj = nullptr;

    unsigned int mObjId;

    GameMap * mGameMap = nullptr;
    ScreenGame * mScreen = nullptr;

    const GameMapCell * mCell = nullptr;

    Player * mOwner = nullptr;
    GameObjectType mType = OBJ_NULL;

    GameObjectActionId mActiveAction = IDLE;

    unsigned int mRows = 1;
    unsigned int mCols = 1;

    int mVisLevel = 0;

    float mMaxHealth = 100.f;
    float mHealth = 100.f;
    float mSpeed = 0.f;

    bool mStructure = false;

    bool mCanBeConq = false;

    bool mSelected = false;

    bool mBusy = false;

    bool mVisible = true;
    bool mVisited = false;

};

inline void GameObject::SetGameMap(GameMap * map) { mGameMap = map; }
inline void GameObject::SetScreen(ScreenGame * screen) { mScreen = screen; }

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

inline bool GameObject::IsDestroyed() const
{
    const float minH = 0.01f;
    return mHealth < minH;
}

inline const GameMapCell * GameObject::GetCell() const { return mCell; }

inline int GameObject::GetVisibilityLevel() const { return mVisLevel; }

inline Player * GameObject::GetOwner() const { return mOwner; }

inline GameObjectType GameObject::GetObjectType() const { return mType; }

inline void GameObject::SetSize(unsigned int rows, unsigned int cols)
{
    mRows = rows;
    mCols = cols;
}

inline unsigned int GameObject::GetRows() const { return mRows; }
inline unsigned int GameObject::GetCols() const { return mCols; }

inline GameMap * GameObject::GetGameMap() const { return mGameMap; }
inline ScreenGame * GameObject::GetScreen() const { return mScreen; }

inline void GameObject::SetVisibilityLevel(int val) { mVisLevel = val; }
inline void GameObject::SetStructure(bool val) { mStructure = val; }
inline void GameObject::SetCanBeConquered(bool val) { mCanBeConq = val; }

inline float GameObject::GetHealth() const { return mHealth; }
inline void GameObject::SumHealth(float val)
{
    mHealth += val;

    if(mHealth > mMaxHealth)
        mHealth = mMaxHealth;
}
inline void GameObject::SetHealth(float health) { mHealth = health; }
inline void GameObject::SetMaxHealth(float max) { mMaxHealth = max; }

inline float GameObject::GetSpeed() const { return mSpeed; }
inline void GameObject::SetSpeed(float speed) { mSpeed = speed; }

inline GameObjectActionId GameObject::GetActiveAction() const { return mActiveAction; }
inline void GameObject::SetActiveAction(GameObjectActionId action) { mActiveAction = action; }

} // namespace game
