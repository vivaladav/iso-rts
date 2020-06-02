#include "graphic/TextureManager.h"

#include "graphic/GraphicConstants.h"
#include "graphic/Texture.h"

namespace lib
{
namespace graphic
{

TextureManager *  TextureManager::mInstance = nullptr;

TextureManager * TextureManager::Create()
{
    if(!mInstance)
        mInstance = new TextureManager;

    return mInstance;
}

void TextureManager::Destroy()
{
    delete mInstance;
}

TextureManager::TextureManager()
    : mTexQuality(TextureQuality::GOOD)
{
}

Texture * TextureManager::GetTexture(const char * file)
{
    const std::string strFile(file);

    Texture * tex = nullptr;

    auto res = mTextures.find(strFile);

    if(res != mTextures.end())
        tex = res->second;
    else
    {
        tex = new Texture(file, mTexQuality);
        mTextures.emplace(strFile, tex);
    }

    return tex;
}

void TextureManager::DestroyTexture(const char * file)
{
    auto res = mTextures.find(std::string(file));

    if(res != mTextures.end())
        delete res->second;

    mTextures.erase(res);
}

void TextureManager::DestroyTextures()
{
    for(auto & item : mTextures)
        delete item.second;

    mTextures.clear();
}

} // namespace graphic
} // namespace lib
