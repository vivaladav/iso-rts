#pragma once

struct SDL_Renderer;

namespace lib
{
namespace graphic
{

class Window;

class Renderer
{
public:
    static Renderer * Create(Window * win);
    static Renderer * Instance();
    static void Destroy();

    void SetRenderColor(unsigned char r, unsigned char g,
                        unsigned char b, unsigned char a);

    void Clear();
    void Clear(unsigned char r, unsigned char g,
               unsigned char b, unsigned char a);

    void Finalize();

    void RenderTexture();

private:
    Renderer(Window * win);
    ~Renderer();

private:
    static Renderer * mInstance;

    SDL_Renderer * mSysRenderer = nullptr;

    // classess that need to access the low level system renderer
    friend class Image;
    friend class Texture;
};


inline Renderer * Renderer::Instance() { return mInstance; }

} // namespace graphic
} // namespace lib
