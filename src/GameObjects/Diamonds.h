#pragma once

#include "GameObject.h"

namespace game
{

class Diamonds : public GameObject
{
public:
    Diamonds();

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
    int mNum = 0;
};

inline Diamonds::Diamonds() : GameObject(GameObjectType::OBJ_DIAMONDS, -1, 1, 1)
{
}

inline int Diamonds::GetNum() const { return mNum; }

} // namespace game
