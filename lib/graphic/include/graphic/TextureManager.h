#pragma once

#include <core/Rect.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace lib
{
namespace graphic
{

class Texture;
class TextureData;

enum TextureQuality : int;

class TextureManager
{
public:
    static TextureManager * Create();
    static TextureManager * Instance();
    static void Destroy();

    // standard Textures
    Texture * GetTexture(const char * file);
    void DestroyTexture(const char * file);
    void DestroyTextures();

    // sprite Textures
    void RegisterSprite(const char * file, const std::vector<core::Rectd> & srcRects);
    Texture * GetSprite(const char * file, unsigned int spriteId);
    void DestroySprites();

    TextureQuality GetNewTextureQuality() const;
    void SetNewTextureQuality(TextureQuality q);

private:
    TextureManager();
    ~TextureManager();

private:
    static TextureManager *  mInstance;

    std::unordered_map<std::string, Texture *> mTextures;

    std::unordered_map<std::string, std::vector<Texture *>> mSprites;

    std::unordered_map<std::string, std::shared_ptr<TextureData>> mTexturesData;

    TextureQuality mTexQuality;
};

inline TextureManager::~TextureManager()
{
    DestroyTextures();
    DestroySprites();
}

inline TextureManager * TextureManager::Instance() { return mInstance; }

inline TextureQuality TextureManager::GetNewTextureQuality() const { return mTexQuality;}
inline void TextureManager::SetNewTextureQuality(TextureQuality q) { mTexQuality = q; }

} // namespace graphic
} // namespace lib
