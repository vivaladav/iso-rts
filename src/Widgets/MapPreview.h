#pragma once

#include <sgui/Widget.h>

namespace lib { namespace graphic { class Image; } }

namespace game
{

class IsoMap;

class MapPreview : public lib::sgui::Widget
{
public:
    MapPreview(lib::sgui::Widget * parent = nullptr);

    bool Load(const char * file);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    IsoMap * mIsoMap = nullptr;

    lib::graphic::Image * mBg = nullptr;
};

} // namespace game
