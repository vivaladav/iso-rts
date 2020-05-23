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

    bool IsFullscreen() const;
    void SetFullscreen(bool f);
    void SwitchFullscreen();

private:
    Window(const char * title, int w, int h);
    ~Window();

private:
    static Window * mInstance;

    SDL_Window * mSysWin = nullptr;

    bool mFullscreen = false;

    // Renderer needs to access the low level window
    friend class Renderer;
};

inline Window * Window::Instance() { return mInstance; }

inline bool Window::IsFullscreen() const { return mFullscreen; }
inline void Window::SwitchFullscreen()
{
    SetFullscreen(!mFullscreen);
}

} // namespace graphic
} // namespace lib
