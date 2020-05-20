#pragma once

#include <string>
#include <unordered_map>

namespace lib
{
namespace graphic
{

class Texture;

class TextureManager
{
public:
    static TextureManager * Create();
    static TextureManager * Instance();
    static void Destroy();

    Texture * GetTexture(const char * file);
    void DestroyTexture(const char * file);
    void DestroyTextures();

private:
    TextureManager();
    ~TextureManager();

private:
    static TextureManager *  mInstance;

    std::unordered_map<std::string, Texture *> mTextures;

};

inline TextureManager::TextureManager() { }
inline TextureManager::~TextureManager() { DestroyTextures(); }

inline TextureManager * TextureManager::Instance() { return mInstance; }

} // namespace graphic
} // namespace lib
