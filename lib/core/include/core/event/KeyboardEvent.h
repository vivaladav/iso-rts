#pragma once

#include "Event.h"

namespace lib
{
namespace core
{

class EventDispatcher;

class KeyboardEvent : public Event
{
public:
    enum EventType : int
    {
        DOWN,
        UP
    };

// == KEYS ==
    static const int KEY_UNKNOWN;

// -- NUMBERS --
    static const int KEY_0;
    static const int KEY_1;
    static const int KEY_2;
    static const int KEY_3;
    static const int KEY_4;
    static const int KEY_5;
    static const int KEY_6;
    static const int KEY_7;
    static const int KEY_8;
    static const int KEY_9;

// -- LETTERS --
    static const int KEY_A;
    static const int KEY_B;
    static const int KEY_C;
    static const int KEY_D;
    static const int KEY_E;
    static const int KEY_F;
    static const int KEY_G;
    static const int KEY_H;
    static const int KEY_I;
    static const int KEY_J;
    static const int KEY_K;
    static const int KEY_L;
    static const int KEY_M;
    static const int KEY_N;
    static const int KEY_O;
    static const int KEY_P;
    static const int KEY_Q;
    static const int KEY_R;
    static const int KEY_S;
    static const int KEY_T;
    static const int KEY_U;
    static const int KEY_V;
    static const int KEY_W;
    static const int KEY_X;
    static const int KEY_Y;
    static const int KEY_Z;

// - KEYPAD --
    static const int KEY_KP_0;
    static const int KEY_KP_00;
    static const int KEY_KP_000;
    static const int KEY_KP_1;
    static const int KEY_KP_2;
    static const int KEY_KP_3;
    static const int KEY_KP_4;
    static const int KEY_KP_5;
    static const int KEY_KP_6;
    static const int KEY_KP_7;
    static const int KEY_KP_8;
    static const int KEY_KP_9;
    static const int KEY_KP_A;
    static const int KEY_KP_B;
    static const int KEY_KP_C;
    static const int KEY_KP_D;
    static const int KEY_KP_E;
    static const int KEY_KP_F;
    static const int KEY_KP_AMPERSAND;
    static const int KEY_KP_AT;
    static const int KEY_KP_BACKSPACE;
    static const int KEY_KP_BINARY;
    static const int KEY_KP_CLEAR;
    static const int KEY_KP_CLEARENTRY;
    static const int KEY_KP_COLON;
    static const int KEY_KP_COMMA;
    static const int KEY_KP_DBLAMPERSAND;
    static const int KEY_KP_DBLVERTICALBAR;
    static const int KEY_KP_DECIMAL;
    static const int KEY_KP_DIVIDE;
    static const int KEY_KP_ENTER;
    static const int KEY_KP_EQUALS;
    static const int KEY_KP_EQUALSAS400;
    static const int KEY_KP_EXCLAM;
    static const int KEY_KP_GREATER;
    static const int KEY_KP_HASH;
    static const int KEY_KP_HEXADECIMAL;
    static const int KEY_KP_LEFTBRACE;
    static const int KEY_KP_LEFTPAREN;
    static const int KEY_KP_LESS;
    static const int KEY_KP_MEMADD;
    static const int KEY_KP_MEMCLEAR;
    static const int KEY_KP_MEMDIVIDE;
    static const int KEY_KP_MEMMULTIPLY;
    static const int KEY_KP_MEMRECALL;
    static const int KEY_KP_MEMSTORE;
    static const int KEY_KP_MEMSUBTRACT;
    static const int KEY_KP_MINUS;
    static const int KEY_KP_MULTIPLY;
    static const int KEY_KP_OCTAL;
    static const int KEY_KP_PERCENT;
    static const int KEY_KP_PERIOD;
    static const int KEY_KP_PLUS;
    static const int KEY_KP_PLUSMINUS;
    static const int KEY_KP_POWER;
    static const int KEY_KP_RIGHTBRACE;
    static const int KEY_KP_RIGHTPAREN;
    static const int KEY_KP_SPACE;
    static const int KEY_KP_TAB;
    static const int KEY_KP_VERTICALBAR;
    static const int KEY_KP_XOR;

// -- F-KEYS --
    static const int KEY_F1;
    static const int KEY_F2;
    static const int KEY_F3;
    static const int KEY_F4;
    static const int KEY_F5;
    static const int KEY_F6;
    static const int KEY_F7;
    static const int KEY_F8;
    static const int KEY_F9;
    static const int KEY_F10;
    static const int KEY_F11;
    static const int KEY_F12;
    static const int KEY_F13;
    static const int KEY_F14;
    static const int KEY_F15;
    static const int KEY_F16;
    static const int KEY_F17;
    static const int KEY_F18;
    static const int KEY_F19;
    static const int KEY_F20;
    static const int KEY_F21;
    static const int KEY_F22;
    static const int KEY_F23;
    static const int KEY_F24;
// -- SYMBOLS --
    static const int KEY_AMPERSAND;
    static const int KEY_ASTERISK;
    static const int KEY_AT;
    static const int KEY_BACKQUOTE;
    static const int KEY_BACKSLASH;
    static const int KEY_CARET;
    static const int KEY_COLON;
    static const int KEY_COMMA;
    static const int KEY_DOLLAR;
    static const int KEY_EQUALS;
    static const int KEY_EXCLAIM;
    static const int KEY_GREATER;
    static const int KEY_HASH;
    static const int KEY_LEFTBRACKET;
    static const int KEY_LEFTPAREN;
    static const int KEY_LESS;
    static const int KEY_MINUS;
    static const int KEY_PERCENT;
    static const int KEY_PERIOD;
    static const int KEY_PLUS;
    static const int KEY_QUESTION;
    static const int KEY_QUOTE;
    static const int KEY_QUOTEDBL;
    static const int KEY_RIGHTBRACKET;
    static const int KEY_RIGHTPAREN;
    static const int KEY_SEMICOLON;
    static const int KEY_SLASH;
    static const int KEY_UNDERSCORE;

// -- FORMATTING --
    static const int KEY_BACKSPACE;
    static const int KEY_DELETE;
    static const int KEY_ESCAPE;
    static const int KEY_RETURN;
    static const int KEY_SPACE;
    static const int KEY_TAB;

// -- MODIFIERS --
    static const int KEY_CAPSLOCK;
    static const int KEY_LALT;
    static const int KEY_LCTRL;
    static const int KEY_LGUI;
    static const int KEY_LSHIFT;
    static const int KEY_MODE;
    static const int KEY_NUMLOCKCLEAR;
    static const int KEY_RALT;
    static const int KEY_RCTRL;
    static const int KEY_RGUI;
    static const int KEY_RSHIFT;

// -- APPLICATION CONTROLS --
    static const int KEY_AC_BACK;
    static const int KEY_AC_BOOKMARKS;
    static const int KEY_AC_FORWARD;
    static const int KEY_AC_HOME;
    static const int KEY_AC_REFRESH;
    static const int KEY_AC_SEARCH;
    static const int KEY_AC_STOP;

// -- NAVIGATION --
    static const int KEY_DOWN;
    static const int KEY_LEFT;
    static const int KEY_PAGEDOWN;
    static const int KEY_PAGEUP;
    static const int KEY_RIGHT;
    static const int KEY_UP;

// -- AUDIO --
    static const int KEY_AUDIOMUTE;
    static const int KEY_AUDIONEXT;
    static const int KEY_AUDIOPLAY;
    static const int KEY_AUDIOPREV;
    static const int KEY_AUDIOSTOP;
    static const int KEY_MEDIASELECT;
    static const int KEY_MUTE;
    static const int KEY_VOLUMEDOWN;
    static const int KEY_VOLUMEUP;

// -- OTHERS --
    static const int KEY_AGAIN;
    static const int KEY_ALTERASE;
    static const int KEY_APPLICATION;
    static const int KEY_BRIGHTNESSDOWN;
    static const int KEY_BRIGHTNESSUP;
    static const int KEY_CALCULATOR;
    static const int KEY_CANCEL;
    static const int KEY_CLEAR;
    static const int KEY_CLEARAGAIN;
    static const int KEY_COMPUTER;
    static const int KEY_COPY;
    static const int KEY_CRSEL;
    static const int KEY_CURRENCYSUBUNIT;
    static const int KEY_CURRENCYUNIT;
    static const int KEY_CUT;
    static const int KEY_DECIMALSEPARATOR;
    static const int KEY_DISPLAYSWITCH;
    static const int KEY_EJECT;
    static const int KEY_END;
    static const int KEY_ESC;
    static const int KEY_EXECUTE;
    static const int KEY_EXSEL;
    static const int KEY_FIND;
    static const int KEY_HELP;
    static const int KEY_HOME;
    static const int KEY_INSERT;
    static const int KEY_KBDILLUMDOWN;
    static const int KEY_KBDILLUMTOGGLE;
    static const int KEY_KBDILLUMUP;
    static const int KEY_MAIL;
    static const int KEY_MENU;
    static const int KEY_OPER;
    static const int KEY_OUT;
    static const int KEY_PASTE;
    static const int KEY_PAUSE;
    static const int KEY_POWER;
    static const int KEY_PRINTSCREEN;
    static const int KEY_PRIOR;
    static const int KEY_RETURN2;
    static const int KEY_SCROLLLOCK;
    static const int KEY_SELECT;
    static const int KEY_SEPARATOR;
    static const int KEY_SLEEP;
    static const int KEY_STOP;
    static const int KEY_SYSREQ;
    static const int KEY_THOUSANDSSEPARATOR;
    static const int KEY_UNDO;
    static const int KEY_WWW;

// == MODIFIERS FLAGS ==
    static const int MOD_ALT;           // LALT or RALT
    static const int MOD_CAPSLOCK;
    static const int MOD_CTRL;          // LCTRL or RCTRL
    static const int MOD_GUI;           // LGUI or RGUI
    static const int MOD_LALT;
    static const int MOD_LCTRL;
    static const int MOD_LGUI;
    static const int MOD_LSHIFT;
    static const int MOD_MODE;
    static const int MOD_NONE;
    static const int MOD_NUMLOCK;
    static const int MOD_RALT;
    static const int MOD_RCTRL;
    static const int MOD_RGUI;
    static const int MOD_RSHIFT;
    static const int MOD_SHIFT;         // LSHIFT or RSHIFT

public:
    int GetType() const;

    int GetKey() const;

private:
    KeyboardEvent(int type, int  key);

private:
    int mType;
    int mKey;

    // only EventDispatcher can create an event
    friend class EventDispatcher;
};


inline KeyboardEvent::KeyboardEvent(int type, int  key)
    : mType(type), mKey(key)
{
}

inline int KeyboardEvent::GetType() const { return mType; }

inline int KeyboardEvent::GetKey() const { return mKey; }

} // namespace core
} // namespace lib
