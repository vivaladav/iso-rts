#include "core/event/KeyboardEvent.h"

#include <SDL2/SDL.h>

namespace lib
{
namespace core
{

// == KEYS ==
const int KeyboardEvent::KEY_UNKNOWN = SDLK_UNKNOWN;

// -- NUMBERS --
const int KeyboardEvent::KEY_0 = SDLK_0;
const int KeyboardEvent::KEY_1 = SDLK_1;
const int KeyboardEvent::KEY_2 = SDLK_2;
const int KeyboardEvent::KEY_3 = SDLK_3;
const int KeyboardEvent::KEY_4 = SDLK_4;
const int KeyboardEvent::KEY_5 = SDLK_5;
const int KeyboardEvent::KEY_6 = SDLK_6;
const int KeyboardEvent::KEY_7 = SDLK_7;
const int KeyboardEvent::KEY_8 = SDLK_8;
const int KeyboardEvent::KEY_9 = SDLK_9;

// -- LETTERS --
const int KeyboardEvent::KEY_A = SDLK_a;
const int KeyboardEvent::KEY_B = SDLK_b;
const int KeyboardEvent::KEY_C = SDLK_c;
const int KeyboardEvent::KEY_D = SDLK_d;
const int KeyboardEvent::KEY_E = SDLK_e;
const int KeyboardEvent::KEY_F = SDLK_f;
const int KeyboardEvent::KEY_G = SDLK_g;
const int KeyboardEvent::KEY_H = SDLK_h;
const int KeyboardEvent::KEY_I = SDLK_i;
const int KeyboardEvent::KEY_J = SDLK_j;
const int KeyboardEvent::KEY_K = SDLK_k;
const int KeyboardEvent::KEY_L = SDLK_l;
const int KeyboardEvent::KEY_M = SDLK_m;
const int KeyboardEvent::KEY_N = SDLK_n;
const int KeyboardEvent::KEY_O = SDLK_o;
const int KeyboardEvent::KEY_P = SDLK_p;
const int KeyboardEvent::KEY_Q = SDLK_q;
const int KeyboardEvent::KEY_R = SDLK_r;
const int KeyboardEvent::KEY_S = SDLK_s;
const int KeyboardEvent::KEY_T = SDLK_t;
const int KeyboardEvent::KEY_U = SDLK_u;
const int KeyboardEvent::KEY_V = SDLK_v;
const int KeyboardEvent::KEY_W = SDLK_w;
const int KeyboardEvent::KEY_X = SDLK_x;
const int KeyboardEvent::KEY_Y = SDLK_y;
const int KeyboardEvent::KEY_Z = SDLK_z;

// -- KEYPAD --
const int KeyboardEvent::KEY_KP_0               = SDLK_KP_0;
const int KeyboardEvent::KEY_KP_00              = SDLK_KP_00;
const int KeyboardEvent::KEY_KP_000             = SDLK_KP_000;
const int KeyboardEvent::KEY_KP_1               = SDLK_KP_1;
const int KeyboardEvent::KEY_KP_2               = SDLK_KP_2;
const int KeyboardEvent::KEY_KP_3               = SDLK_KP_3;
const int KeyboardEvent::KEY_KP_4               = SDLK_KP_4;
const int KeyboardEvent::KEY_KP_5               = SDLK_KP_5;
const int KeyboardEvent::KEY_KP_6               = SDLK_KP_6;
const int KeyboardEvent::KEY_KP_7               = SDLK_KP_7;
const int KeyboardEvent::KEY_KP_8               = SDLK_KP_8;
const int KeyboardEvent::KEY_KP_9               = SDLK_KP_9;
const int KeyboardEvent::KEY_KP_A               = SDLK_KP_A;
const int KeyboardEvent::KEY_KP_B               = SDLK_KP_B;
const int KeyboardEvent::KEY_KP_C               = SDLK_KP_C;
const int KeyboardEvent::KEY_KP_D               = SDLK_KP_D;
const int KeyboardEvent::KEY_KP_E               = SDLK_KP_E;
const int KeyboardEvent::KEY_KP_F               = SDLK_KP_F;
const int KeyboardEvent::KEY_KP_AMPERSAND       = SDLK_KP_AMPERSAND;
const int KeyboardEvent::KEY_KP_AT              = SDLK_KP_AT;
const int KeyboardEvent::KEY_KP_BACKSPACE       = SDLK_KP_BACKSPACE;
const int KeyboardEvent::KEY_KP_BINARY          = SDLK_KP_BINARY;
const int KeyboardEvent::KEY_KP_CLEAR           = SDLK_KP_CLEAR;
const int KeyboardEvent::KEY_KP_CLEARENTRY      = SDLK_KP_CLEARENTRY;
const int KeyboardEvent::KEY_KP_COLON           = SDLK_KP_COLON;
const int KeyboardEvent::KEY_KP_COMMA           = SDLK_KP_COMMA;
const int KeyboardEvent::KEY_KP_DBLAMPERSAND    = SDLK_KP_DBLAMPERSAND;
const int KeyboardEvent::KEY_KP_DBLVERTICALBAR  = SDLK_KP_DBLVERTICALBAR;
const int KeyboardEvent::KEY_KP_DECIMAL         = SDLK_KP_DECIMAL;
const int KeyboardEvent::KEY_KP_DIVIDE          = SDLK_KP_DIVIDE;
const int KeyboardEvent::KEY_KP_ENTER           = SDLK_KP_ENTER;
const int KeyboardEvent::KEY_KP_EQUALS          = SDLK_KP_EQUALS;
const int KeyboardEvent::KEY_KP_EQUALSAS400     = SDLK_KP_EQUALSAS400;
const int KeyboardEvent::KEY_KP_EXCLAM          = SDLK_KP_EXCLAM;
const int KeyboardEvent::KEY_KP_GREATER         = SDLK_KP_GREATER;
const int KeyboardEvent::KEY_KP_HASH            = SDLK_KP_HASH;
const int KeyboardEvent::KEY_KP_HEXADECIMAL     = SDLK_KP_HEXADECIMAL;
const int KeyboardEvent::KEY_KP_LEFTBRACE       = SDLK_KP_LEFTBRACE;
const int KeyboardEvent::KEY_KP_LEFTPAREN       = SDLK_KP_LEFTPAREN;
const int KeyboardEvent::KEY_KP_LESS            = SDLK_KP_LESS;
const int KeyboardEvent::KEY_KP_MEMADD          = SDLK_KP_MEMADD;
const int KeyboardEvent::KEY_KP_MEMCLEAR        = SDLK_KP_MEMCLEAR;
const int KeyboardEvent::KEY_KP_MEMDIVIDE       = SDLK_KP_MEMDIVIDE;
const int KeyboardEvent::KEY_KP_MEMMULTIPLY     = SDLK_KP_MEMMULTIPLY;
const int KeyboardEvent::KEY_KP_MEMRECALL       = SDLK_KP_MEMRECALL;
const int KeyboardEvent::KEY_KP_MEMSTORE        = SDLK_KP_MEMSTORE;
const int KeyboardEvent::KEY_KP_MEMSUBTRACT     = SDLK_KP_MEMSUBTRACT;
const int KeyboardEvent::KEY_KP_MINUS           = SDLK_KP_MINUS;
const int KeyboardEvent::KEY_KP_MULTIPLY        = SDLK_KP_MULTIPLY;
const int KeyboardEvent::KEY_KP_OCTAL           = SDLK_KP_OCTAL;
const int KeyboardEvent::KEY_KP_PERCENT         = SDLK_KP_PERCENT;
const int KeyboardEvent::KEY_KP_PERIOD          = SDLK_KP_PERIOD;
const int KeyboardEvent::KEY_KP_PLUS            = SDLK_KP_PLUS;
const int KeyboardEvent::KEY_KP_PLUSMINUS       = SDLK_KP_PLUSMINUS;
const int KeyboardEvent::KEY_KP_POWER           = SDLK_KP_POWER;
const int KeyboardEvent::KEY_KP_RIGHTBRACE      = SDLK_KP_RIGHTBRACE;
const int KeyboardEvent::KEY_KP_RIGHTPAREN      = SDLK_KP_RIGHTPAREN;
const int KeyboardEvent::KEY_KP_SPACE           = SDLK_KP_SPACE;
const int KeyboardEvent::KEY_KP_TAB             = SDLK_KP_TAB;
const int KeyboardEvent::KEY_KP_VERTICALBAR     = SDLK_KP_VERTICALBAR;
const int KeyboardEvent::KEY_KP_XOR             = SDLK_KP_XOR;

// -- F-KEYS --
const int KeyboardEvent::KEY_F1     = SDLK_F1;
const int KeyboardEvent::KEY_F2     = SDLK_F2;
const int KeyboardEvent::KEY_F3     = SDLK_F3;
const int KeyboardEvent::KEY_F4     = SDLK_F4;
const int KeyboardEvent::KEY_F5     = SDLK_F5;
const int KeyboardEvent::KEY_F6     = SDLK_F6;
const int KeyboardEvent::KEY_F7     = SDLK_F7;
const int KeyboardEvent::KEY_F8     = SDLK_F8;
const int KeyboardEvent::KEY_F9     = SDLK_F9;
const int KeyboardEvent::KEY_F10    = SDLK_F10;
const int KeyboardEvent::KEY_F11    = SDLK_F11;
const int KeyboardEvent::KEY_F12    = SDLK_F12;
const int KeyboardEvent::KEY_F13    = SDLK_F13;
const int KeyboardEvent::KEY_F14    = SDLK_F14;
const int KeyboardEvent::KEY_F15    = SDLK_F15;
const int KeyboardEvent::KEY_F16    = SDLK_F16;
const int KeyboardEvent::KEY_F17    = SDLK_F17;
const int KeyboardEvent::KEY_F18    = SDLK_F18;
const int KeyboardEvent::KEY_F19    = SDLK_F19;
const int KeyboardEvent::KEY_F20    = SDLK_F20;
const int KeyboardEvent::KEY_F21    = SDLK_F21;
const int KeyboardEvent::KEY_F22    = SDLK_F22;
const int KeyboardEvent::KEY_F23    = SDLK_F23;
const int KeyboardEvent::KEY_F24    = SDLK_F24;

// -- SYMBOLS --
const int KeyboardEvent::KEY_AMPERSAND      = SDLK_AMPERSAND;
const int KeyboardEvent::KEY_ASTERISK       = SDLK_ASTERISK;
const int KeyboardEvent::KEY_AT             = SDLK_AT;
const int KeyboardEvent::KEY_BACKQUOTE      = SDLK_BACKQUOTE;
const int KeyboardEvent::KEY_BACKSLASH      = SDLK_BACKSLASH;
const int KeyboardEvent::KEY_CARET          = SDLK_CARET;
const int KeyboardEvent::KEY_COLON          = SDLK_COLON;
const int KeyboardEvent::KEY_COMMA          = SDLK_COMMA;
const int KeyboardEvent::KEY_DOLLAR         = SDLK_DOLLAR;
const int KeyboardEvent::KEY_EQUALS         = SDLK_EQUALS;
const int KeyboardEvent::KEY_EXCLAIM        = SDLK_EXCLAIM;
const int KeyboardEvent::KEY_GREATER        = SDLK_GREATER;
const int KeyboardEvent::KEY_HASH           = SDLK_HASH;
const int KeyboardEvent::KEY_LEFTBRACKET    = SDLK_LEFTBRACKET;
const int KeyboardEvent::KEY_LEFTPAREN      = SDLK_LEFTPAREN;
const int KeyboardEvent::KEY_LESS           = SDLK_LESS;
const int KeyboardEvent::KEY_MINUS          = SDLK_MINUS;
const int KeyboardEvent::KEY_PERCENT        = SDLK_PERCENT;
const int KeyboardEvent::KEY_PERIOD         = SDLK_PERIOD;
const int KeyboardEvent::KEY_PLUS           = SDLK_PLUS;
const int KeyboardEvent::KEY_QUESTION       = SDLK_QUESTION;
const int KeyboardEvent::KEY_QUOTE          = SDLK_QUOTE;
const int KeyboardEvent::KEY_QUOTEDBL       = SDLK_QUOTEDBL;
const int KeyboardEvent::KEY_RIGHTBRACKET   = SDLK_RIGHTBRACKET;
const int KeyboardEvent::KEY_RIGHTPAREN     = SDLK_RIGHTPAREN;
const int KeyboardEvent::KEY_SEMICOLON      = SDLK_SEMICOLON;
const int KeyboardEvent::KEY_SLASH          = SDLK_SLASH;
const int KeyboardEvent::KEY_UNDERSCORE     = SDLK_UNDERSCORE;

// -- FORMATTING --
const int KeyboardEvent::KEY_BACKSPACE      = SDLK_BACKSPACE;
const int KeyboardEvent::KEY_DELETE         = SDLK_DELETE;
const int KeyboardEvent::KEY_ESCAPE         = SDLK_ESCAPE;
const int KeyboardEvent::KEY_RETURN         = SDLK_RETURN;
const int KeyboardEvent::KEY_SPACE          = SDLK_SPACE;
const int KeyboardEvent::KEY_TAB            = SDLK_TAB;

// -- MODIFIERS --
const int KeyboardEvent::KEY_CAPSLOCK       = SDLK_CAPSLOCK;
const int KeyboardEvent::KEY_LALT           = SDLK_LALT;
const int KeyboardEvent::KEY_LCTRL          = SDLK_LCTRL;
const int KeyboardEvent::KEY_LGUI           = SDLK_LGUI;
const int KeyboardEvent::KEY_LSHIFT         = SDLK_LSHIFT;
const int KeyboardEvent::KEY_MODE           = SDLK_MODE;
const int KeyboardEvent::KEY_NUMLOCKCLEAR   = SDLK_NUMLOCKCLEAR;
const int KeyboardEvent::KEY_RALT           = SDLK_RALT;
const int KeyboardEvent::KEY_RCTRL          = SDLK_RCTRL;
const int KeyboardEvent::KEY_RGUI           = SDLK_RGUI;
const int KeyboardEvent::KEY_RSHIFT         = SDLK_RSHIFT;

// -- APPLICATION CONTROLS --
const int KeyboardEvent::KEY_AC_BACK        = SDLK_AC_BACK;
const int KeyboardEvent::KEY_AC_BOOKMARKS   = SDLK_AC_BOOKMARKS;
const int KeyboardEvent::KEY_AC_FORWARD     = SDLK_AC_FORWARD;
const int KeyboardEvent::KEY_AC_HOME        = SDLK_AC_HOME;
const int KeyboardEvent::KEY_AC_REFRESH     = SDLK_AC_REFRESH;
const int KeyboardEvent::KEY_AC_SEARCH      = SDLK_AC_SEARCH;
const int KeyboardEvent::KEY_AC_STOP        = SDLK_AC_STOP;

// -- NAVIGATION --
const int KeyboardEvent::KEY_DOWN       = SDLK_DOWN;
const int KeyboardEvent::KEY_LEFT       = SDLK_LEFT;
const int KeyboardEvent::KEY_PAGEDOWN   = SDLK_PAGEDOWN;
const int KeyboardEvent::KEY_PAGEUP     = SDLK_PAGEUP;
const int KeyboardEvent::KEY_RIGHT      = SDLK_RIGHT;
const int KeyboardEvent::KEY_UP         = SDLK_UP;

// -- AUDIO --
const int KeyboardEvent::KEY_AUDIOMUTE      = SDLK_AUDIOMUTE;
const int KeyboardEvent::KEY_AUDIONEXT      = SDLK_AUDIONEXT;
const int KeyboardEvent::KEY_AUDIOPLAY      = SDLK_AUDIOPLAY;
const int KeyboardEvent::KEY_AUDIOPREV      = SDLK_AUDIOPREV;
const int KeyboardEvent::KEY_AUDIOSTOP      = SDLK_AUDIOSTOP;
const int KeyboardEvent::KEY_MEDIASELECT    = SDLK_MEDIASELECT;
const int KeyboardEvent::KEY_MUTE           = SDLK_MUTE;
const int KeyboardEvent::KEY_VOLUMEDOWN     = SDLK_VOLUMEDOWN;
const int KeyboardEvent::KEY_VOLUMEUP       = SDLK_VOLUMEUP;

// -- OTHERS --
const int KeyboardEvent::KEY_AGAIN              = SDLK_AGAIN;
const int KeyboardEvent::KEY_ALTERASE           = SDLK_ALTERASE;
const int KeyboardEvent::KEY_APPLICATION        = SDLK_APPLICATION;
const int KeyboardEvent::KEY_BRIGHTNESSDOWN     = SDLK_BRIGHTNESSDOWN;
const int KeyboardEvent::KEY_BRIGHTNESSUP       = SDLK_BRIGHTNESSUP;
const int KeyboardEvent::KEY_CALCULATOR         = SDLK_CALCULATOR;
const int KeyboardEvent::KEY_CANCEL             = SDLK_CANCEL;
const int KeyboardEvent::KEY_CLEAR              = SDLK_CLEAR;
const int KeyboardEvent::KEY_CLEARAGAIN         = SDLK_CLEARAGAIN;
const int KeyboardEvent::KEY_COMPUTER           = SDLK_COMPUTER;
const int KeyboardEvent::KEY_COPY               = SDLK_COPY;
const int KeyboardEvent::KEY_CRSEL              = SDLK_CRSEL;
const int KeyboardEvent::KEY_CURRENCYSUBUNIT    = SDLK_CURRENCYSUBUNIT;
const int KeyboardEvent::KEY_CURRENCYUNIT       = SDLK_CURRENCYUNIT;
const int KeyboardEvent::KEY_CUT                = SDLK_CUT;
const int KeyboardEvent::KEY_DECIMALSEPARATOR   = SDLK_DECIMALSEPARATOR;
const int KeyboardEvent::KEY_DISPLAYSWITCH      = SDLK_DISPLAYSWITCH;
const int KeyboardEvent::KEY_EJECT              = SDLK_EJECT;
const int KeyboardEvent::KEY_END                = SDLK_END;
const int KeyboardEvent::KEY_ESC                = SDLK_ESCAPE;
const int KeyboardEvent::KEY_EXECUTE            = SDLK_EXECUTE;
const int KeyboardEvent::KEY_EXSEL              = SDLK_EXSEL;
const int KeyboardEvent::KEY_FIND               = SDLK_FIND;
const int KeyboardEvent::KEY_HELP               = SDLK_HELP;
const int KeyboardEvent::KEY_HOME               = SDLK_HOME;
const int KeyboardEvent::KEY_INSERT             = SDLK_INSERT;
const int KeyboardEvent::KEY_KBDILLUMDOWN       = SDLK_KBDILLUMDOWN;
const int KeyboardEvent::KEY_KBDILLUMTOGGLE     = SDLK_KBDILLUMTOGGLE;
const int KeyboardEvent::KEY_KBDILLUMUP         = SDLK_KBDILLUMUP;
const int KeyboardEvent::KEY_MAIL               = SDLK_MAIL;
const int KeyboardEvent::KEY_MENU               = SDLK_MENU;
const int KeyboardEvent::KEY_OPER               = SDLK_OPER;
const int KeyboardEvent::KEY_OUT                = SDLK_OUT;
const int KeyboardEvent::KEY_PASTE              = SDLK_PASTE;
const int KeyboardEvent::KEY_PAUSE              = SDLK_PAUSE;
const int KeyboardEvent::KEY_POWER              = SDLK_POWER;
const int KeyboardEvent::KEY_PRINTSCREEN        = SDLK_PRINTSCREEN;
const int KeyboardEvent::KEY_PRIOR              = SDLK_PRIOR;
const int KeyboardEvent::KEY_RETURN2            = SDLK_RETURN2;
const int KeyboardEvent::KEY_SCROLLLOCK         = SDLK_SCROLLLOCK;
const int KeyboardEvent::KEY_SELECT             = SDLK_SELECT;
const int KeyboardEvent::KEY_SEPARATOR          = SDLK_SEPARATOR;
const int KeyboardEvent::KEY_SLEEP              = SDLK_SLEEP;
const int KeyboardEvent::KEY_STOP               = SDLK_STOP;
const int KeyboardEvent::KEY_SYSREQ             = SDLK_SYSREQ;
const int KeyboardEvent::KEY_THOUSANDSSEPARATOR = SDLK_THOUSANDSSEPARATOR;
const int KeyboardEvent::KEY_UNDO               = SDLK_UNDO;
const int KeyboardEvent::KEY_WWW                = SDLK_WWW;

// == MODIFIERS FLAGS ==
const int KeyboardEvent::MOD_ALT         = KMOD_ALT;        // LALT or RALT
const int KeyboardEvent::MOD_CAPSLOCK    = KMOD_CAPS;
const int KeyboardEvent::MOD_CTRL        = KMOD_CTRL;       // LCTRL or RCTRL
const int KeyboardEvent::MOD_GUI         = KMOD_GUI;        // LGUI or RGUI
const int KeyboardEvent::MOD_LALT        = KMOD_LALT;
const int KeyboardEvent::MOD_LCTRL       = KMOD_LCTRL;
const int KeyboardEvent::MOD_LGUI        = KMOD_LGUI;
const int KeyboardEvent::MOD_LSHIFT      = KMOD_LSHIFT;
const int KeyboardEvent::MOD_MODE        = KMOD_MODE;
const int KeyboardEvent::MOD_NUMLOCK     = KMOD_NUM;
const int KeyboardEvent::MOD_RALT        = KMOD_RALT;
const int KeyboardEvent::MOD_RCTRL       = KMOD_RCTRL;
const int KeyboardEvent::MOD_RGUI        = KMOD_RGUI;
const int KeyboardEvent::MOD_RSHIFT      = KMOD_RSHIFT;
const int KeyboardEvent::MOD_SHIFT       = KMOD_SHIFT;      // LSHIFT or RSHIFT
const int KeyboardEvent::MOD_NONE        = KMOD_NONE;

// == PRIVATE FUNCTIOns ==
KeyboardEvent::KeyboardEvent(int key, unsigned int modifiers)
    : mKey(key)
    , mModAltDown(modifiers & KMOD_ALT)
    , mModCtrlDown(modifiers & KMOD_CTRL)
    , mModGuiDown(modifiers & KMOD_GUI)
    , mModShiftDown(modifiers & KMOD_SHIFT)
{
}

} // namespace core
} // namespace lib
