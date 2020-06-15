#include "graphic/Image.h"

#include "graphic/Texture.h"
#include "graphic/TextureManager.h"

#include <SDL2/SDL.h>

#include <cassert>

namespace lib
{
namespace graphic
{

Image::Image(const char * file)
    : Image()
{
    Load(file);
}

bool Image::Load(const char * file)
{
    assert(file);

    Texture * tex = TextureManager::Instance()->GetTexture(file);

    if(!tex)
        return false;

    SetTexture(tex);

    return true;
}


} // namespace graphic
} // namespace lib
