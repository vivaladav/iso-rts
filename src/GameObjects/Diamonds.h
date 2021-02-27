#pragma once

#include "GameObject.h"

namespace game
{

class DiamondsGenerator;

class Diamonds : public GameObject
{
public:
    Diamonds();
    ~Diamonds();

    void SetGenerator(DiamondsGenerator * gen);

    int GetNum() const;
    void SetNum(int num);

public:
    static const int MIN_UNITS = 1;
    static const int MAX_UNITS = 4;

protected:
    void UpdateImage() override;

private:
    void SetImage();

private:
    DiamondsGenerator * mGen = nullptr;

    int mNum = 0;
};

inline Diamonds::Diamonds() : GameObject(GameObjectType::OBJ_DIAMONDS, -1, 1, 1)
{
}

inline void Diamonds::SetGenerator(DiamondsGenerator * gen) { mGen = gen; }

inline int Diamonds::GetNum() const { return mNum; }

} // namespace game
