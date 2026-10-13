#pragma once

#include "GameObject.h"

namespace game
{

enum ExtendedResource : unsigned int;

class LootBox : public GameObject
{
public:
    LootBox(const ObjectData & data, const ObjectInitData & initData);

    bool Load(sgl::utilities::BinaryFile & bf) override;
    bool Save(sgl::utilities::BinaryFile & bf) const override;

    bool IsExploding() const;

    void Open(Player * p);

private:
    void UpdateGraphics() override;

    void SetPrize();

    void SetImage();
    void SetObjColors();

private:
    friend class StepGameSetLootboxPrize;

    int mPrizeQuantity = 0;
    ExtendedResource mPrizeType;
};


} // namespace game
