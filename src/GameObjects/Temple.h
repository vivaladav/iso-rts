#pragma once

#include "Structure.h"

namespace game
{

class Temple : public Structure
{
public:
    enum ExplorationOutcomeCategory : unsigned int
    {
        EXP_OUTC_GOOD,
        EXP_OUTC_NOTHING,
        EXP_OUTC_BAD,

        NUM_EXPLORATION_OUTCOME_CATEGORIES,

        EXP_OUTC_NULL
    };

    enum ExplorationOutcome : unsigned int
    {
        // -- REWARDS --
        // INCREASE EXISTING RESOURCES
        EXP_REW_MULT10_MONEY,
        EXP_REW_MAX_RES_ENERGY,
        EXP_REW_MAX_RES_MATERIAL,
        EXP_REW_MAX_RES_BLOBS,
        EXP_REW_MAX_RES_DIAMONDS,
        EXP_REW_MAX_RESOURCES,
        // INCREASE PRODUCTION
        EXP_REW_INC_ENERGY_PRODUCTION,
        EXP_REW_INC_MATERIAL_PRODUCTION,
        // MAXIMIZE COLLECTIBLES
        EXP_REW_MAX_BLOBS,
        EXP_REW_MAX_DIAMONDS,

        // -- PUNISHMENTS --
        // DECREASE EXISTING RESOURCES
        EXP_PUN_ZERO_MONEY,
        EXP_PUN_ZERO_RES_ENERGY,
        EXP_PUN_ZERO_RES_MATERIAL,
        EXP_PUN_ZERO_RES_BLOBS,
        EXP_PUN_ZERO_RES_DIAMONDS,
        EXP_PUN_ZERO_RESOURCES,
        // DECREASE PRODUCTION
        EXP_PUN_DEC_ENERGY_PRODUCTION,
        EXP_PUN_DEC_MATERIAL_PRODUCTION,
        // MINIMIZE COLLECTIBLES
        EXP_PUN_MIN_BLOBS,
        EXP_PUN_MIN_DIAMONDS,

        NUM_EXPLORATION_OUTCOMES,

        // FIRST/LAST VALUES - update when adding new outcomes
        FIRST_EXP_REW = EXP_REW_MULT10_MONEY,
        LAST_EXP_REW = EXP_REW_MAX_DIAMONDS,
        NUM_EXP_REW = LAST_EXP_REW - FIRST_EXP_REW + 1,
        FIRST_EXP_PUN = EXP_PUN_ZERO_MONEY,
        LAST_EXP_PUN = EXP_PUN_MIN_DIAMONDS,
        NUM_EXP_PUN = LAST_EXP_PUN - FIRST_EXP_PUN + 1,

        // NULL
        EXP_OUT_NULL
    };

public:
    Temple();

    void SetInvestedResources(int money, int material, int blobs, int diamonds);
    int GetExplorationTime() const;
    int GetExplorationSuccessRate() const;

    void Explore();
    ExplorationOutcomeCategory GetExplorationOutcomeCategory() const;
    void GetExplorationOucomeCouple(ExplorationOutcome & o1, ExplorationOutcome & o2);
    const char * GetExplorationOutcomeString(ExplorationOutcome o) const;

    int GetMaxInvestableMoney() const;
    int GetMaxInvestableMaterial() const;
    int GetMaxInvestableBlobs() const;
    int GetMaxInvestableDiamonds() const;

private:
    void DecideRewards();
    void DecidePunishments();

    void UpdateGraphics() override;

    void SetImage();
    void SetObjColors();

private:
    int mExplorationTime = 0;
    int mExplorationSuccess = 0;

    ExplorationOutcomeCategory mOutcomeCat = EXP_OUTC_NULL;
    ExplorationOutcome mOutcome1 = EXP_OUT_NULL;
    ExplorationOutcome mOutcome2 = EXP_OUT_NULL;
};

inline int Temple::GetExplorationTime() const { return mExplorationTime; }
inline int Temple::GetExplorationSuccessRate() const { return mExplorationSuccess; }


inline Temple::ExplorationOutcomeCategory Temple::GetExplorationOutcomeCategory() const
{
    return mOutcomeCat;
}

inline void Temple::GetExplorationOucomeCouple(ExplorationOutcome & o1, ExplorationOutcome & o2)
{
    o1 = mOutcome1;
    o2 = mOutcome2;
}

inline int Temple::GetMaxInvestableMoney() const { return 5000; }
inline int Temple::GetMaxInvestableMaterial() const { return 5000; }
inline int Temple::GetMaxInvestableBlobs() const { return 400; }
inline int Temple::GetMaxInvestableDiamonds() const { return 400; }

} // namespace game
