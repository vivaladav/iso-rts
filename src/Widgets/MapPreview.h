#pragma once

#include <sgui/Widget.h>

#include <functional>

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

    void SetSelected(bool val);

    void SetOnClickFunction(const std::function<void()> & f);

private:
    void HandleMouseButtonUp(lib::core::MouseButtonEvent & event) override;
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    IsoMap * mIsoMap = nullptr;

    lib::graphic::Image * mBg = nullptr;
    lib::graphic::Image * mBgSel = nullptr;
    lib::graphic::Image * mBgUnsel = nullptr;
    lib::graphic::Text * mLabelSize = nullptr;
};

inline void MapPreview::SetOnClickFunction(const std::function<void()> & f) { mOnClick = f; }

} // namespace game
