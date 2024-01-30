#pragma once

#include "Collectable.h"

namespace game
{

class BlobsGenerator;

class Blobs : public Collectable
{
public:
    Blobs();

    int GetNum() const;
    void MaximizeUnits();
    void MinimizeUnits();

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();

private:
    int mNum = 0;
};

inline int Blobs::GetNum() const { return mNum; }

} // namespace game
