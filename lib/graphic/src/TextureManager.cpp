#include "graphic/TextureManager.h"

#include "graphic/GraphicConstants.h"
#include "graphic/Texture.h"
#include "graphic/TextureData.h"

#include <iostream>

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

void TextureManager::RegisterSprite(const char * file, const std::vector<core::Rectd> & srcRects)
{
    const std::string strFile(file);

    auto res = mSprites.find(strFile);

    // vector with Textures already created
    if(res != mSprites.end())
    {
        std::cout << "TextureManager::RegisterSprite - ERR: Sprite " << file
                  << " already registered" << std::endl;
        return ;
    }

    const unsigned int numRects = srcRects.size();

    std::vector<Texture *> textures;

    // create Textures with no data
    for(unsigned int i = 0; i < numRects; ++i)
    {
        auto tex = new Texture;
        tex->SetSourceRect(srcRects[i]);

        textures.push_back(tex);
    }

    // store textures in map
    mSprites.emplace(strFile, textures);

    // create null entry for data
    mTexturesData.emplace(strFile, nullptr);
}

Texture * TextureManager::GetSprite(const char * file, unsigned int spriteId)
{
    const std::string strFile(file);

    // find atlas
    auto res = mSprites.find(strFile);

    // no atlas found
    if(mSprites.end() == res)
        return nullptr;

    // find texture data
    auto resData = mTexturesData.find(strFile);

    // no texture data found (this should never happen
    if(mTexturesData.end() == resData)
        return nullptr;

    const std::vector<Texture *> & textures = res->second;

    // no sprite found
    if(spriteId >= textures.size())
        return nullptr;

    Texture * tex = textures[spriteId];

    // handle data
    if(!tex->HasData())
    {
        std::shared_ptr<TextureData> data = resData->second;

        if(nullptr == data)
        {
            data = std::make_shared<TextureData>(file, mTexQuality);
            resData->second = data;
        }

        tex->SetData(data);
    }

    return tex;
}

void TextureManager::DestroySprites()
{
    for(auto & sprite : mSprites)
    {
        for(auto tex : sprite.second)
            delete tex;
    }

    mSprites.clear();

    mTexturesData.clear();
}

} // namespace graphic
} // namespace lib
