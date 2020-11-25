#pragma once

#include <sgui/Widget.h>

namespace lib
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class IsoMap;

class MapPreview : public lib::sgui::Widget
{
public:
    MapPreview(const char * file, lib::sgui::Widget * parent = nullptr);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    IsoMap * mIsoMap = nullptr;

    lib::graphic::Image * mBg = nullptr;
    lib::graphic::Text * mLabelSize = nullptr;
};

} // namespace game
