#pragma once

#include "Tutorial/TutorialInfoStep.h"

#include <unordered_map>

namespace sgl { namespace sgui { class AbstractButton; } }

namespace game
{

class DialogTrading;
class Game;
class FocusArea;
class Player;

enum ExtendedResource : unsigned int;

class StepGameDialogTradingBuy : public TutorialInfoStep
{
public:
    StepGameDialogTradingBuy(const Game * game, const Player * p, const DialogTrading * dialog);
    ~StepGameDialogTradingBuy();

    void OnStart() override;

private:
    unsigned int GetQuantityToBuy(ExtendedResource res, unsigned int max, int spent) const;

    void HandleButton(unsigned int res, unsigned int value);

private:
    FocusArea * mFocusArea = nullptr;

    std::unordered_map<sgl::sgui::AbstractButton *, unsigned int> mCallbacks;

    const DialogTrading * mDialog = nullptr;

    const Game * mGame = nullptr;
    const Player * mPlayer = nullptr;
};

} // namespace game
