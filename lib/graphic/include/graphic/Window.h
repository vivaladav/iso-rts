#pragma once

struct SDL_Window;

namespace lib
{
namespace graphic
{

class Renderer;

class Window
{
public:
    static Window * Create(const char * title, int w, int h);
    static Window * Instance();
    static void Destroy();

    void SetSize(int w, int h);

private:
    Window(const char * title, int w, int h);
    ~Window();

private:
    static Window * mInstance;

    SDL_Window * mSysWin = nullptr;

    // Renderer needs to access the low level window
    friend class Renderer;
};

inline Window * Window::Instance() { return mInstance; }

} // namespace graphic
} // namespace lib
