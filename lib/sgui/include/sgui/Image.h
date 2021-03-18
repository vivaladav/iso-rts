#pragma once

#include "sgui/Widget.h"

#include <string>

namespace lib
{

namespace graphic { class Renderable; }

namespace sgui
{

class Image : public Widget
{
public:
    Image(Widget * parent = nullptr);
    Image(const char * file, Widget * parent = nullptr);
    ~Image();

    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetColor(unsigned int color);

    void LoadImage(const char * file);

private:
    void HandlePositionChanged() override;

    void OnRender() override;

private:
    graphic::Renderable * mImg = nullptr;
};

} // namespace sgui
} // namespace lib
