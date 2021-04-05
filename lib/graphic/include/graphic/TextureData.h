#pragma once

#include <string>

struct SDL_Surface;
struct SDL_Texture;

namespace lib
{
namespace graphic
{

enum TextureQuality : int;

class TextureData
{
public:
    TextureData(SDL_Surface * data, TextureQuality q);
    TextureData(const char * file, TextureQuality q);
    ~TextureData();

    const std::string & GetFilePath() const;

    int GetWidth() const;
    int GetHeight() const;

    void SetAlpha(unsigned char a);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    void SetTextureQuality(TextureQuality q);

private:
    std::string mFilePath;

    SDL_Texture * mData = nullptr;

    int mWidth = 0;
    int mHeight = 0;

    // access SDL Texture data
    friend class TexturedRenderable;
};

inline const std::string & TextureData::GetFilePath() const { return mFilePath; }

inline int TextureData::GetWidth() const { return mWidth; }
inline int TextureData::GetHeight() const { return mHeight; }

} // namespace graphic
} // namespace lib
