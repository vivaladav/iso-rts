#pragma once

#include <sgl/sgui/Widget.h>

#include <functional>
#include <string>

namespace sgl
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

class MapPreview : public sgl::sgui::Widget
{
public:
    MapPreview(const std::string & file, sgl::sgui::Widget * parent = nullptr);

    void SetSelected(bool val);

    void SetOnClickFunction(const std::function<void()> & f);

private:
    void HandleMouseButtonUp(sgl::core::MouseButtonEvent & event) override;
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    std::function<void()> mOnClick;

    IsoMap * mIsoMap = nullptr;

    sgl::graphic::Image * mBg = nullptr;
    sgl::graphic::Image * mBgSel = nullptr;
    sgl::graphic::Image * mBgUnsel = nullptr;
    sgl::graphic::Text * mLabelSize = nullptr;
};

inline void MapPreview::SetOnClickFunction(const std::function<void()> & f) { mOnClick = f; }

} // namespace game
