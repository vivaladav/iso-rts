#pragma once

#include <string>
#include <unordered_map>

namespace lib
{
namespace graphic
{

class Texture;

enum TextureQuality : int;

class TextureManager
{
public:
    static TextureManager * Create();
    static TextureManager * Instance();
    static void Destroy();

    Texture * GetTexture(const char * file);
    void DestroyTexture(const char * file);
    void DestroyTextures();

    TextureQuality GetNewTextureQuality() const;
    void SetNewTextureQuality(TextureQuality q);

private:
    TextureManager();
    ~TextureManager();

private:
    static TextureManager *  mInstance;

    std::unordered_map<std::string, Texture *> mTextures;

    TextureQuality mTexQuality;
};

inline TextureManager::~TextureManager() { DestroyTextures(); }

inline TextureManager * TextureManager::Instance() { return mInstance; }

inline TextureQuality TextureManager::GetNewTextureQuality() const { return mTexQuality;}
inline void TextureManager::SetNewTextureQuality(TextureQuality q) { mTexQuality = q; }

} // namespace graphic
} // namespace lib
