#pragma once

#include "GameObjectTypes.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace game
{

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
    BUILD_STRUCTURE,
    BUILD_WALL,

    // WALL GATE
    TOGGLE_GATE,

    NUM_OBJ_ACTIONS
};

class GameMap;
class IsoObject;
class Player;
class ScreenGame;
struct GameMapCell;

class GameObject
{
public:
    static const GameObjectTypeId TYPE_NULL;

    static const GameObjectTypeId TYPE_BASE;
    static const GameObjectTypeId TYPE_BLOBS;
    static const GameObjectTypeId TYPE_DEFENSIVE_TOWER;
    static const GameObjectTypeId TYPE_DIAMONDS;
    static const GameObjectTypeId TYPE_MOUNTAINS;
    static const GameObjectTypeId TYPE_PRACTICE_TARGET;
    static const GameObjectTypeId TYPE_RADAR_STATION;
    static const GameObjectTypeId TYPE_RADAR_TOWER;
    static const GameObjectTypeId TYPE_RES_GEN_ENERGY;
    static const GameObjectTypeId TYPE_RES_GEN_ENERGY_SOLAR;
    static const GameObjectTypeId TYPE_RES_GEN_MATERIAL;
    static const GameObjectTypeId TYPE_RES_GEN_MATERIAL_EXTRACT;
    static const GameObjectTypeId TYPE_RES_STORAGE_BLOBS;
    static const GameObjectTypeId TYPE_RES_STORAGE_DIAMONDS;
    static const GameObjectTypeId TYPE_RES_STORAGE_ENERGY;
    static const GameObjectTypeId TYPE_RES_STORAGE_MATERIAL;
    static const GameObjectTypeId TYPE_ROCKS;
    static const GameObjectTypeId TYPE_TREES;
    static const GameObjectTypeId TYPE_UNIT;
    static const GameObjectTypeId TYPE_WALL;
    static const GameObjectTypeId TYPE_WALL_GATE;

    static const std::unordered_map<GameObjectTypeId, std::string> TITLES;
    static const std::unordered_map<GameObjectTypeId, std::string> DESCRIPTIONS;

public:
    GameObject(GameObjectTypeId type, int rows, int cols);
    virtual ~GameObject();

    void SetGameMap(GameMap * map);
    void SetScreen(ScreenGame * screen);

    unsigned int GetObjectId() const;

    IsoObject * GetIsoObject() const;

    bool IsStructure() const;
    bool CanBeCollected() const;
    bool CanBeConquered() const;

    bool IsSelected() const;
    void SetSelected(bool val);

    bool IsBusy() const;
    void SetBusy(bool val);

    bool IsVisited() const;
    void SetVisited();

    bool IsVisible() const;
    void SetVisible(bool val);

    bool IsLinked() const;
    void SetLinked(bool val);

    bool IsStatic() const;

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

    GameObjectTypeId GetObjectType() const;

    float GetHealth() const;
    float GetMaxHealth() const;
    void SetMaxHealth(float max);
    void SetHealth(float val);
    void SumHealth(float val);

    float GetEnergy() const;
    void SetEnergy(float val);
    void SumEnergy(float val);
    float GetMaxEnergy() const;
    void SetMaxEnergy(float val);

    void SetOnValuesChanged(const std::function<void()> & f);

    float GetSpeed() const;

    void Hit(float damage);

    GameObjectActionId GetActiveAction() const;
    void SetActiveAction(GameObjectActionId action);
    virtual void SetActiveActionToDefault();
    GameObjectActionId GetCurrentAction() const;
    void SetCurrentAction(GameObjectActionId action);

    virtual void Update(float delta);

protected:
    virtual void UpdateGraphics() = 0;

    virtual void OnLinkedChanged();

    void SetDefaultColors();

    GameMap * GetGameMap() const;
    ScreenGame * GetScreen() const;

    void SetVisibilityLevel(int val);
    void SetStructure(bool val);
    void SetCanBeCollected(bool val);
    void SetCanBeConquered(bool val);
    void SetStatic(bool val);

    void SetSpeed(float speed);

protected:
    static const unsigned int COLOR_FOW;
    static const unsigned int COLOR_VIS;

    std::vector<unsigned int> mObjColors;

private:
    static unsigned int counter;

private:
    std::function<void()> mOnValuesChanged;

    IsoObject * mIsoObj = nullptr;

    unsigned int mObjId;

    GameMap * mGameMap = nullptr;
    ScreenGame * mScreen = nullptr;

    const GameMapCell * mCell = nullptr;

    Player * mOwner = nullptr;
    GameObjectTypeId mType = TYPE_NULL;

    GameObjectActionId mActiveAction = IDLE;
    GameObjectActionId mCurrAction = IDLE;

    unsigned int mRows = 1;
    unsigned int mCols = 1;

    int mVisLevel = 0;

    float mMaxEnergy = 100.f;
    float mEnergy = 100.f;
    float mMaxHealth = 100.f;
    float mHealth = 100.f;
    float mSpeed = 0.f;

    bool mStructure = false;
    // set to TRUE for elements that stay visible under FOGOW
    bool mStatic = false;

    bool mCanBeCollected = false;
    bool mCanBeConq = false;

    bool mSelected = false;

    bool mBusy = false;

    bool mVisible = true;
    bool mVisited = false;
    bool mLinked = false;
};

inline void GameObject::SetGameMap(GameMap * map) { mGameMap = map; }
inline void GameObject::SetScreen(ScreenGame * screen) { mScreen = screen; }

inline unsigned int GameObject::GetObjectId() const { return mObjId; }

inline IsoObject * GameObject::GetIsoObject() const { return mIsoObj; }

inline bool GameObject::IsStructure() const { return mStructure; }

inline bool GameObject::CanBeCollected() const { return mCanBeCollected; }
inline bool GameObject::CanBeConquered() const { return mCanBeConq; }

inline bool GameObject::IsSelected() const { return mSelected; }

inline bool GameObject::IsBusy() const { return mBusy; }
inline void GameObject::SetBusy(bool val) { mBusy = val; }

inline bool GameObject::IsVisited() const { return mVisited; }
inline void GameObject::SetVisited() { mVisited = true; }

inline bool GameObject::IsVisible() const { return mVisible; }

inline bool GameObject::IsLinked() const { return mLinked; }

inline bool GameObject::IsStatic() const { return mStatic; }

inline bool GameObject::IsDestroyed() const
{
    const float minH = 0.01f;
    return mHealth < minH;
}

inline const GameMapCell * GameObject::GetCell() const { return mCell; }

inline int GameObject::GetVisibilityLevel() const { return mVisLevel; }

inline Player * GameObject::GetOwner() const { return mOwner; }

inline GameObjectTypeId GameObject::GetObjectType() const { return mType; }

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
inline void GameObject::SetCanBeCollected(bool val) { mCanBeCollected = val; }
inline void GameObject::SetCanBeConquered(bool val) { mCanBeConq = val; }
inline void GameObject::SetStatic(bool val) { mStatic = val; }

inline float GameObject::GetHealth() const { return mHealth; }
inline float GameObject::GetMaxHealth() const { return mMaxHealth; }
inline void GameObject::SetMaxHealth(float max) { mMaxHealth = max; }

inline float GameObject::GetEnergy() const { return mEnergy; }
inline float GameObject::GetMaxEnergy() const { return mMaxEnergy; }
inline void GameObject::SetMaxEnergy(float val) { mMaxEnergy = val; }

inline void GameObject::SetOnValuesChanged(const std::function<void()> & f)
{
    mOnValuesChanged = f;
}

inline float GameObject::GetSpeed() const { return mSpeed; }
inline void GameObject::SetSpeed(float speed) { mSpeed = speed; }

inline GameObjectActionId GameObject::GetActiveAction() const { return mActiveAction; }
inline void GameObject::SetActiveAction(GameObjectActionId action) { mActiveAction = action; }
inline GameObjectActionId GameObject::GetCurrentAction() const { return mCurrAction; }
inline void GameObject::SetCurrentAction(GameObjectActionId action) { mCurrAction = action; }

} // namespace game
