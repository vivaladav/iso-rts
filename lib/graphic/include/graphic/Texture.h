#pragma once

struct SDL_Surface;
struct SDL_Texture;

namespace lib
{
namespace graphic
{

class TexturedRenderable;

enum TextureQuality : int;

class Texture
{
public:
    Texture(SDL_Surface * data, TextureQuality q);
    Texture(const char * file, TextureQuality q);
    ~Texture();

    int GetWidth() const;
    int GetHeight() const;

    void SetAlpha(unsigned char a);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    void SetTextureQuality(TextureQuality q);

private:
    SDL_Texture * mData = nullptr;

    int mW = 0;
    int mH = 0;

    // access SDL Texture data
    friend class TexturedRenderable;
};

inline int Texture::GetWidth() const { return mW; }
inline int Texture::GetHeight() const { return mH; }

} // namespace graphic
} // namespace lib
