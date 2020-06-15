#pragma once

#include "graphic/TexturedRenderable.h"

namespace lib
{
namespace graphic
{

class Image : public TexturedRenderable
{
public:
    Image();
    Image(const char * file);

    bool Load(const char * file);
};

inline Image::Image() { }

} // namespace graphic
} // namespace lib
