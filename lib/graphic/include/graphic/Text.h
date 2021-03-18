#pragma once

#include "graphic/TexturedRenderable.h"

namespace lib
{
namespace graphic
{

class Font;

class Text : public TexturedRenderable
{
public:
    Text(const char * text, Font * font, bool trim = false);
    ~Text();
};

} // namespace graphic
} // namespace lib

