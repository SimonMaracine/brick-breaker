#pragma once

/*
    From SDL2 source code
*/

#define BB_INTERNAL_SCANCODE_TO_KEYCODE(X) ((X) | (1 << 30))
#define BB_INTERNAL_BUTTON(X) (1 << ((X) - 1))

namespace bb {
    enum class MouseButton : unsigned char {
        LEFT = 1,
        MIDDLE = 2,
        RIGHT = 3,
        X1 = 4,
        X2 = 5
    };

    enum MouseButtons : unsigned int {
        MB_LEFT = BB_INTERNAL_BUTTON(1),
        MB_MIDDLE = BB_INTERNAL_BUTTON(2),
        MB_RIGHT = BB_INTERNAL_BUTTON(3),
        MB_X1 = BB_INTERNAL_BUTTON(4),
        MB_X2 = BB_INTERNAL_BUTTON(5)
    };

    namespace internal {
        enum InternalScancode {
            BB_INTERNAL_SCANCODE_UNKNOWN = 0,

            BB_INTERNAL_SCANCODE_A = 4,
            BB_INTERNAL_SCANCODE_B = 5,
            BB_INTERNAL_SCANCODE_C = 6,
            BB_INTERNAL_SCANCODE_D = 7,
            BB_INTERNAL_SCANCODE_E = 8,
            BB_INTERNAL_SCANCODE_F = 9,
            BB_INTERNAL_SCANCODE_G = 10,
            BB_INTERNAL_SCANCODE_H = 11,
            BB_INTERNAL_SCANCODE_I = 12,
            BB_INTERNAL_SCANCODE_J = 13,
            BB_INTERNAL_SCANCODE_K = 14,
            BB_INTERNAL_SCANCODE_L = 15,
            BB_INTERNAL_SCANCODE_M = 16,
            BB_INTERNAL_SCANCODE_N = 17,
            BB_INTERNAL_SCANCODE_O = 18,
            BB_INTERNAL_SCANCODE_P = 19,
            BB_INTERNAL_SCANCODE_Q = 20,
            BB_INTERNAL_SCANCODE_R = 21,
            BB_INTERNAL_SCANCODE_S = 22,
            BB_INTERNAL_SCANCODE_T = 23,
            BB_INTERNAL_SCANCODE_U = 24,
            BB_INTERNAL_SCANCODE_V = 25,
            BB_INTERNAL_SCANCODE_W = 26,
            BB_INTERNAL_SCANCODE_X = 27,
            BB_INTERNAL_SCANCODE_Y = 28,
            BB_INTERNAL_SCANCODE_Z = 29,

            BB_INTERNAL_SCANCODE_1 = 30,
            BB_INTERNAL_SCANCODE_2 = 31,
            BB_INTERNAL_SCANCODE_3 = 32,
            BB_INTERNAL_SCANCODE_4 = 33,
            BB_INTERNAL_SCANCODE_5 = 34,
            BB_INTERNAL_SCANCODE_6 = 35,
            BB_INTERNAL_SCANCODE_7 = 36,
            BB_INTERNAL_SCANCODE_8 = 37,
            BB_INTERNAL_SCANCODE_9 = 38,
            BB_INTERNAL_SCANCODE_0 = 39,

            BB_INTERNAL_SCANCODE_RETURN = 40,
            BB_INTERNAL_SCANCODE_ESCAPE = 41,
            BB_INTERNAL_SCANCODE_BACKSPACE = 42,
            BB_INTERNAL_SCANCODE_TAB = 43,
            BB_INTERNAL_SCANCODE_SPACE = 44,

            BB_INTERNAL_SCANCODE_MINUS = 45,
            BB_INTERNAL_SCANCODE_EQUALS = 46,
            BB_INTERNAL_SCANCODE_LEFTBRACKET = 47,
            BB_INTERNAL_SCANCODE_RIGHTBRACKET = 48,
            BB_INTERNAL_SCANCODE_BACKSLASH = 49,
            BB_INTERNAL_SCANCODE_NONUSHASH = 50,
            BB_INTERNAL_SCANCODE_SEMICOLON = 51,
            BB_INTERNAL_SCANCODE_APOSTROPHE = 52,
            BB_INTERNAL_SCANCODE_GRAVE = 53,
            BB_INTERNAL_SCANCODE_COMMA = 54,
            BB_INTERNAL_SCANCODE_PERIOD = 55,
            BB_INTERNAL_SCANCODE_SLASH = 56,

            BB_INTERNAL_SCANCODE_CAPSLOCK = 57,

            BB_INTERNAL_SCANCODE_F1 = 58,
            BB_INTERNAL_SCANCODE_F2 = 59,
            BB_INTERNAL_SCANCODE_F3 = 60,
            BB_INTERNAL_SCANCODE_F4 = 61,
            BB_INTERNAL_SCANCODE_F5 = 62,
            BB_INTERNAL_SCANCODE_F6 = 63,
            BB_INTERNAL_SCANCODE_F7 = 64,
            BB_INTERNAL_SCANCODE_F8 = 65,
            BB_INTERNAL_SCANCODE_F9 = 66,
            BB_INTERNAL_SCANCODE_F10 = 67,
            BB_INTERNAL_SCANCODE_F11 = 68,
            BB_INTERNAL_SCANCODE_F12 = 69,

            BB_INTERNAL_SCANCODE_PRINTSCREEN = 70,
            BB_INTERNAL_SCANCODE_SCROLLLOCK = 71,
            BB_INTERNAL_SCANCODE_PAUSE = 72,
            BB_INTERNAL_SCANCODE_INSERT = 73,
            BB_INTERNAL_SCANCODE_HOME = 74,
            BB_INTERNAL_SCANCODE_PAGEUP = 75,
            BB_INTERNAL_SCANCODE_DELETE = 76,
            BB_INTERNAL_SCANCODE_END = 77,
            BB_INTERNAL_SCANCODE_PAGEDOWN = 78,
            BB_INTERNAL_SCANCODE_RIGHT = 79,
            BB_INTERNAL_SCANCODE_LEFT = 80,
            BB_INTERNAL_SCANCODE_DOWN = 81,
            BB_INTERNAL_SCANCODE_UP = 82,

            BB_INTERNAL_SCANCODE_NUMLOCKCLEAR = 83,
            BB_INTERNAL_SCANCODE_KP_DIVIDE = 84,
            BB_INTERNAL_SCANCODE_KP_MULTIPLY = 85,
            BB_INTERNAL_SCANCODE_KP_MINUS = 86,
            BB_INTERNAL_SCANCODE_KP_PLUS = 87,
            BB_INTERNAL_SCANCODE_KP_ENTER = 88,
            BB_INTERNAL_SCANCODE_KP_1 = 89,
            BB_INTERNAL_SCANCODE_KP_2 = 90,
            BB_INTERNAL_SCANCODE_KP_3 = 91,
            BB_INTERNAL_SCANCODE_KP_4 = 92,
            BB_INTERNAL_SCANCODE_KP_5 = 93,
            BB_INTERNAL_SCANCODE_KP_6 = 94,
            BB_INTERNAL_SCANCODE_KP_7 = 95,
            BB_INTERNAL_SCANCODE_KP_8 = 96,
            BB_INTERNAL_SCANCODE_KP_9 = 97,
            BB_INTERNAL_SCANCODE_KP_0 = 98,
            BB_INTERNAL_SCANCODE_KP_PERIOD = 99,

            BB_INTERNAL_SCANCODE_NONUSBACKSLASH = 100,
            BB_INTERNAL_SCANCODE_APPLICATION = 101,
            BB_INTERNAL_SCANCODE_POWER = 102,
            BB_INTERNAL_SCANCODE_KP_EQUALS = 103,
            BB_INTERNAL_SCANCODE_F13 = 104,
            BB_INTERNAL_SCANCODE_F14 = 105,
            BB_INTERNAL_SCANCODE_F15 = 106,
            BB_INTERNAL_SCANCODE_F16 = 107,
            BB_INTERNAL_SCANCODE_F17 = 108,
            BB_INTERNAL_SCANCODE_F18 = 109,
            BB_INTERNAL_SCANCODE_F19 = 110,
            BB_INTERNAL_SCANCODE_F20 = 111,
            BB_INTERNAL_SCANCODE_F21 = 112,
            BB_INTERNAL_SCANCODE_F22 = 113,
            BB_INTERNAL_SCANCODE_F23 = 114,
            BB_INTERNAL_SCANCODE_F24 = 115,
            BB_INTERNAL_SCANCODE_EXECUTE = 116,
            BB_INTERNAL_SCANCODE_HELP = 117,
            BB_INTERNAL_SCANCODE_MENU = 118,
            BB_INTERNAL_SCANCODE_SELECT = 119,
            BB_INTERNAL_SCANCODE_STOP = 120,
            BB_INTERNAL_SCANCODE_AGAIN = 121,
            BB_INTERNAL_SCANCODE_UNDO = 122,
            BB_INTERNAL_SCANCODE_CUT = 123,
            BB_INTERNAL_SCANCODE_COPY = 124,
            BB_INTERNAL_SCANCODE_PASTE = 125,
            BB_INTERNAL_SCANCODE_FIND = 126,
            BB_INTERNAL_SCANCODE_MUTE = 127,
            BB_INTERNAL_SCANCODE_VOLUMEUP = 128,
            BB_INTERNAL_SCANCODE_VOLUMEDOWN = 129,
            BB_INTERNAL_SCANCODE_KP_COMMA = 133,
            BB_INTERNAL_SCANCODE_KP_EQUALSAS400 = 134,

            BB_INTERNAL_SCANCODE_INTERNATIONAL1 = 135,
            BB_INTERNAL_SCANCODE_INTERNATIONAL2 = 136,
            BB_INTERNAL_SCANCODE_INTERNATIONAL3 = 137,
            BB_INTERNAL_SCANCODE_INTERNATIONAL4 = 138,
            BB_INTERNAL_SCANCODE_INTERNATIONAL5 = 139,
            BB_INTERNAL_SCANCODE_INTERNATIONAL6 = 140,
            BB_INTERNAL_SCANCODE_INTERNATIONAL7 = 141,
            BB_INTERNAL_SCANCODE_INTERNATIONAL8 = 142,
            BB_INTERNAL_SCANCODE_INTERNATIONAL9 = 143,
            BB_INTERNAL_SCANCODE_LANG1 = 144,
            BB_INTERNAL_SCANCODE_LANG2 = 145,
            BB_INTERNAL_SCANCODE_LANG3 = 146,
            BB_INTERNAL_SCANCODE_LANG4 = 147,
            BB_INTERNAL_SCANCODE_LANG5 = 148,
            BB_INTERNAL_SCANCODE_LANG6 = 149,
            BB_INTERNAL_SCANCODE_LANG7 = 150,
            BB_INTERNAL_SCANCODE_LANG8 = 151,
            BB_INTERNAL_SCANCODE_LANG9 = 152,

            BB_INTERNAL_SCANCODE_ALTERASE = 153,
            BB_INTERNAL_SCANCODE_SYSREQ = 154,
            BB_INTERNAL_SCANCODE_CANCEL = 155,
            BB_INTERNAL_SCANCODE_CLEAR = 156,
            BB_INTERNAL_SCANCODE_PRIOR = 157,
            BB_INTERNAL_SCANCODE_RETURN2 = 158,
            BB_INTERNAL_SCANCODE_SEPARATOR = 159,
            BB_INTERNAL_SCANCODE_OUT = 160,
            BB_INTERNAL_SCANCODE_OPER = 161,
            BB_INTERNAL_SCANCODE_CLEARAGAIN = 162,
            BB_INTERNAL_SCANCODE_CRSEL = 163,
            BB_INTERNAL_SCANCODE_EXSEL = 164,

            BB_INTERNAL_SCANCODE_KP_00 = 176,
            BB_INTERNAL_SCANCODE_KP_000 = 177,
            BB_INTERNAL_SCANCODE_THOUSANDSSEPARATOR = 178,
            BB_INTERNAL_SCANCODE_DECIMALSEPARATOR = 179,
            BB_INTERNAL_SCANCODE_CURRENCYUNIT = 180,
            BB_INTERNAL_SCANCODE_CURRENCYSUBUNIT = 181,
            BB_INTERNAL_SCANCODE_KP_LEFTPAREN = 182,
            BB_INTERNAL_SCANCODE_KP_RIGHTPAREN = 183,
            BB_INTERNAL_SCANCODE_KP_LEFTBRACE = 184,
            BB_INTERNAL_SCANCODE_KP_RIGHTBRACE = 185,
            BB_INTERNAL_SCANCODE_KP_TAB = 186,
            BB_INTERNAL_SCANCODE_KP_BACKSPACE = 187,
            BB_INTERNAL_SCANCODE_KP_A = 188,
            BB_INTERNAL_SCANCODE_KP_B = 189,
            BB_INTERNAL_SCANCODE_KP_C = 190,
            BB_INTERNAL_SCANCODE_KP_D = 191,
            BB_INTERNAL_SCANCODE_KP_E = 192,
            BB_INTERNAL_SCANCODE_KP_F = 193,
            BB_INTERNAL_SCANCODE_KP_XOR = 194,
            BB_INTERNAL_SCANCODE_KP_POWER = 195,
            BB_INTERNAL_SCANCODE_KP_PERCENT = 196,
            BB_INTERNAL_SCANCODE_KP_LESS = 197,
            BB_INTERNAL_SCANCODE_KP_GREATER = 198,
            BB_INTERNAL_SCANCODE_KP_AMPERSAND = 199,
            BB_INTERNAL_SCANCODE_KP_DBLAMPERSAND = 200,
            BB_INTERNAL_SCANCODE_KP_VERTICALBAR = 201,
            BB_INTERNAL_SCANCODE_KP_DBLVERTICALBAR = 202,
            BB_INTERNAL_SCANCODE_KP_COLON = 203,
            BB_INTERNAL_SCANCODE_KP_HASH = 204,
            BB_INTERNAL_SCANCODE_KP_SPACE = 205,
            BB_INTERNAL_SCANCODE_KP_AT = 206,
            BB_INTERNAL_SCANCODE_KP_EXCLAM = 207,
            BB_INTERNAL_SCANCODE_KP_MEMSTORE = 208,
            BB_INTERNAL_SCANCODE_KP_MEMRECALL = 209,
            BB_INTERNAL_SCANCODE_KP_MEMCLEAR = 210,
            BB_INTERNAL_SCANCODE_KP_MEMADD = 211,
            BB_INTERNAL_SCANCODE_KP_MEMSUBTRACT = 212,
            BB_INTERNAL_SCANCODE_KP_MEMMULTIPLY = 213,
            BB_INTERNAL_SCANCODE_KP_MEMDIVIDE = 214,
            BB_INTERNAL_SCANCODE_KP_PLUSMINUS = 215,
            BB_INTERNAL_SCANCODE_KP_CLEAR = 216,
            BB_INTERNAL_SCANCODE_KP_CLEARENTRY = 217,
            BB_INTERNAL_SCANCODE_KP_BINARY = 218,
            BB_INTERNAL_SCANCODE_KP_OCTAL = 219,
            BB_INTERNAL_SCANCODE_KP_DECIMAL = 220,
            BB_INTERNAL_SCANCODE_KP_HEXADECIMAL = 221,

            BB_INTERNAL_SCANCODE_LCTRL = 224,
            BB_INTERNAL_SCANCODE_LSHIFT = 225,
            BB_INTERNAL_SCANCODE_LALT = 226,
            BB_INTERNAL_SCANCODE_LGUI = 227,
            BB_INTERNAL_SCANCODE_RCTRL = 228,
            BB_INTERNAL_SCANCODE_RSHIFT = 229,
            BB_INTERNAL_SCANCODE_RALT = 230,
            BB_INTERNAL_SCANCODE_RGUI = 231,

            BB_INTERNAL_SCANCODE_MODE = 257,

            BB_INTERNAL_SCANCODE_AUDIONEXT = 258,
            BB_INTERNAL_SCANCODE_AUDIOPREV = 259,
            BB_INTERNAL_SCANCODE_AUDIOSTOP = 260,
            BB_INTERNAL_SCANCODE_AUDIOPLAY = 261,
            BB_INTERNAL_SCANCODE_AUDIOMUTE = 262,
            BB_INTERNAL_SCANCODE_MEDIASELECT = 263,
            BB_INTERNAL_SCANCODE_WWW = 264,
            BB_INTERNAL_SCANCODE_MAIL = 265,
            BB_INTERNAL_SCANCODE_CALCULATOR = 266,
            BB_INTERNAL_SCANCODE_COMPUTER = 267,
            BB_INTERNAL_SCANCODE_AC_SEARCH = 268,
            BB_INTERNAL_SCANCODE_AC_HOME = 269,
            BB_INTERNAL_SCANCODE_AC_BACK = 270,
            BB_INTERNAL_SCANCODE_AC_FORWARD = 271,
            BB_INTERNAL_SCANCODE_AC_STOP = 272,
            BB_INTERNAL_SCANCODE_AC_REFRESH = 273,
            BB_INTERNAL_SCANCODE_AC_BOOKMARKS = 274,

            BB_INTERNAL_SCANCODE_BRIGHTNESSDOWN = 275,
            BB_INTERNAL_SCANCODE_BRIGHTNESSUP = 276,
            BB_INTERNAL_SCANCODE_DISPLAYSWITCH = 277,
            BB_INTERNAL_SCANCODE_KBDILLUMTOGGLE = 278,
            BB_INTERNAL_SCANCODE_KBDILLUMDOWN = 279,
            BB_INTERNAL_SCANCODE_KBDILLUMUP = 280,
            BB_INTERNAL_SCANCODE_EJECT = 281,
            BB_INTERNAL_SCANCODE_SLEEP = 282,

            BB_INTERNAL_SCANCODE_APP1 = 283,
            BB_INTERNAL_SCANCODE_APP2 = 284,

            BB_INTERNAL_SCANCODE_AUDIOREWIND = 285,
            BB_INTERNAL_SCANCODE_AUDIOFASTFORWARD = 286,

            BB_INTERNAL_SCANCODE_SOFTLEFT = 287,
            BB_INTERNAL_SCANCODE_SOFTRIGHT = 288,
            BB_INTERNAL_SCANCODE_CALL = 289,
            BB_INTERNAL_SCANCODE_ENDCALL = 290,

            BB_INTERNAL_NUM_SCANCODES = 512
        };
    }

    enum class KeyCode {
        K_UNKNOWN = 0,

        K_RETURN = '\r',
        K_ESCAPE = '\x1B',
        K_BACKSPACE = '\b',
        K_TAB = '\t',
        K_SPACE = ' ',
        K_EXCLAIM = '!',
        K_QUOTEDBL = '"',
        K_HASH = '#',
        K_PERCENT = '%',
        K_DOLLAR = '$',
        K_AMPERSAND = '&',
        K_QUOTE = '\'',
        K_LEFTPAREN = '(',
        K_RIGHTPAREN = ')',
        K_ASTERISK = '*',
        K_PLUS = '+',
        K_COMMA = ',',
        K_MINUS = '-',
        K_PERIOD = '.',
        K_SLASH = '/',
        K_0 = '0',
        K_1 = '1',
        K_2 = '2',
        K_3 = '3',
        K_4 = '4',
        K_5 = '5',
        K_6 = '6',
        K_7 = '7',
        K_8 = '8',
        K_9 = '9',
        K_COLON = ':',
        K_SEMICOLON = ';',
        K_LESS = '<',
        K_EQUALS = '=',
        K_GREATER = '>',
        K_QUESTION = '?',
        K_AT = '@',

        K_LEFTBRACKET = '[',
        K_BACKSLASH = '\\',
        K_RIGHTBRACKET = ']',
        K_CARET = '^',
        K_UNDERSCORE = '_',
        K_BACKQUOTE = '`',
        K_a = 'a',
        K_b = 'b',
        K_c = 'c',
        K_d = 'd',
        K_e = 'e',
        K_f = 'f',
        K_g = 'g',
        K_h = 'h',
        K_i = 'i',
        K_j = 'j',
        K_k = 'k',
        K_l = 'l',
        K_m = 'm',
        K_n = 'n',
        K_o = 'o',
        K_p = 'p',
        K_q = 'q',
        K_r = 'r',
        K_s = 's',
        K_t = 't',
        K_u = 'u',
        K_v = 'v',
        K_w = 'w',
        K_x = 'x',
        K_y = 'y',
        K_z = 'z',

        K_CAPSLOCK = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CAPSLOCK),

        K_F1 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F1),
        K_F2 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F2),
        K_F3 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F3),
        K_F4 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F4),
        K_F5 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F5),
        K_F6 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F6),
        K_F7 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F7),
        K_F8 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F8),
        K_F9 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F9),
        K_F10 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F10),
        K_F11 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F11),
        K_F12 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F12),

        K_PRINTSCREEN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_PRINTSCREEN),
        K_SCROLLLOCK = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SCROLLLOCK),
        K_PAUSE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_PAUSE),
        K_INSERT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_INSERT),
        K_HOME = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_HOME),
        K_PAGEUP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_PAGEUP),
        K_DELETE = '\x7F',
        K_END = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_END),
        K_PAGEDOWN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_PAGEDOWN),
        K_RIGHT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_RIGHT),
        K_LEFT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_LEFT),
        K_DOWN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_DOWN),
        K_UP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_UP),

        K_NUMLOCKCLEAR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_NUMLOCKCLEAR),
        K_KP_DIVIDE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_DIVIDE),
        K_KP_MULTIPLY = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MULTIPLY),
        K_KP_MINUS = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MINUS),
        K_KP_PLUS = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_PLUS),
        K_KP_ENTER = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_ENTER),
        K_KP_1 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_1),
        K_KP_2 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_2),
        K_KP_3 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_3),
        K_KP_4 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_4),
        K_KP_5 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_5),
        K_KP_6 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_6),
        K_KP_7 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_7),
        K_KP_8 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_8),
        K_KP_9 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_9),
        K_KP_0 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_0),
        K_KP_PERIOD = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_PERIOD),

        K_APPLICATION = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_APPLICATION),
        K_POWER = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_POWER),
        K_KP_EQUALS = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_EQUALS),
        K_F13 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F13),
        K_F14 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F14),
        K_F15 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F15),
        K_F16 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F16),
        K_F17 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F17),
        K_F18 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F18),
        K_F19 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F19),
        K_F20 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F20),
        K_F21 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F21),
        K_F22 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F22),
        K_F23 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F23),
        K_F24 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_F24),
        K_EXECUTE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_EXECUTE),
        K_HELP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_HELP),
        K_MENU = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_MENU),
        K_SELECT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SELECT),
        K_STOP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_STOP),
        K_AGAIN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AGAIN),
        K_UNDO = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_UNDO),
        K_CUT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CUT),
        K_COPY = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_COPY),
        K_PASTE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_PASTE),
        K_FIND = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_FIND),
        K_MUTE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_MUTE),
        K_VOLUMEUP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_VOLUMEUP),
        K_VOLUMEDOWN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_VOLUMEDOWN),
        K_KP_COMMA = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_COMMA),
        K_KP_EQUALSAS400 =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_EQUALSAS400),

        K_ALTERASE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_ALTERASE),
        K_SYSREQ = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SYSREQ),
        K_CANCEL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CANCEL),
        K_CLEAR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CLEAR),
        K_PRIOR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_PRIOR),
        K_RETURN2 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_RETURN2),
        K_SEPARATOR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SEPARATOR),
        K_OUT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_OUT),
        K_OPER = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_OPER),
        K_CLEARAGAIN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CLEARAGAIN),
        K_CRSEL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CRSEL),
        K_EXSEL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_EXSEL),

        K_KP_00 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_00),
        K_KP_000 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_000),
        K_THOUSANDSSEPARATOR =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_THOUSANDSSEPARATOR),
        K_DECIMALSEPARATOR =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_DECIMALSEPARATOR),
        K_CURRENCYUNIT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CURRENCYUNIT),
        K_CURRENCYSUBUNIT =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CURRENCYSUBUNIT),
        K_KP_LEFTPAREN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_LEFTPAREN),
        K_KP_RIGHTPAREN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_RIGHTPAREN),
        K_KP_LEFTBRACE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_LEFTBRACE),
        K_KP_RIGHTBRACE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_RIGHTBRACE),
        K_KP_TAB = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_TAB),
        K_KP_BACKSPACE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_BACKSPACE),
        K_KP_A = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_A),
        K_KP_B = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_B),
        K_KP_C = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_C),
        K_KP_D = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_D),
        K_KP_E = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_E),
        K_KP_F = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_F),
        K_KP_XOR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_XOR),
        K_KP_POWER = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_POWER),
        K_KP_PERCENT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_PERCENT),
        K_KP_LESS = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_LESS),
        K_KP_GREATER = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_GREATER),
        K_KP_AMPERSAND = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_AMPERSAND),
        K_KP_DBLAMPERSAND =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_DBLAMPERSAND),
        K_KP_VERTICALBAR =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_VERTICALBAR),
        K_KP_DBLVERTICALBAR =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_DBLVERTICALBAR),
        K_KP_COLON = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_COLON),
        K_KP_HASH = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_HASH),
        K_KP_SPACE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_SPACE),
        K_KP_AT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_AT),
        K_KP_EXCLAM = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_EXCLAM),
        K_KP_MEMSTORE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMSTORE),
        K_KP_MEMRECALL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMRECALL),
        K_KP_MEMCLEAR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMCLEAR),
        K_KP_MEMADD = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMADD),
        K_KP_MEMSUBTRACT =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMSUBTRACT),
        K_KP_MEMMULTIPLY =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMMULTIPLY),
        K_KP_MEMDIVIDE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_MEMDIVIDE),
        K_KP_PLUSMINUS = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_PLUSMINUS),
        K_KP_CLEAR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_CLEAR),
        K_KP_CLEARENTRY = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_CLEARENTRY),
        K_KP_BINARY = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_BINARY),
        K_KP_OCTAL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_OCTAL),
        K_KP_DECIMAL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_DECIMAL),
        K_KP_HEXADECIMAL =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KP_HEXADECIMAL),

        K_LCTRL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_LCTRL),
        K_LSHIFT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_LSHIFT),
        K_LALT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_LALT),
        K_LGUI = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_LGUI),
        K_RCTRL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_RCTRL),
        K_RSHIFT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_RSHIFT),
        K_RALT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_RALT),
        K_RGUI = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_RGUI),

        K_MODE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_MODE),

        K_AUDIONEXT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIONEXT),
        K_AUDIOPREV = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIOPREV),
        K_AUDIOSTOP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIOSTOP),
        K_AUDIOPLAY = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIOPLAY),
        K_AUDIOMUTE = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIOMUTE),
        K_MEDIASELECT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_MEDIASELECT),
        K_WWW = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_WWW),
        K_MAIL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_MAIL),
        K_CALCULATOR = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CALCULATOR),
        K_COMPUTER = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_COMPUTER),
        K_AC_SEARCH = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_SEARCH),
        K_AC_HOME = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_HOME),
        K_AC_BACK = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_BACK),
        K_AC_FORWARD = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_FORWARD),
        K_AC_STOP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_STOP),
        K_AC_REFRESH = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_REFRESH),
        K_AC_BOOKMARKS = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AC_BOOKMARKS),

        K_BRIGHTNESSDOWN =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_BRIGHTNESSDOWN),
        K_BRIGHTNESSUP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_BRIGHTNESSUP),
        K_DISPLAYSWITCH = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_DISPLAYSWITCH),
        K_KBDILLUMTOGGLE =
            BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KBDILLUMTOGGLE),
        K_KBDILLUMDOWN = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KBDILLUMDOWN),
        K_KBDILLUMUP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_KBDILLUMUP),
        K_EJECT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_EJECT),
        K_SLEEP = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SLEEP),
        K_APP1 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_APP1),
        K_APP2 = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_APP2),

        K_AUDIOREWIND = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIOREWIND),
        K_AUDIOFASTFORWARD = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_AUDIOFASTFORWARD),

        K_SOFTLEFT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SOFTLEFT),
        K_SOFTRIGHT = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_SOFTRIGHT),
        K_CALL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_CALL),
        K_ENDCALL = BB_INTERNAL_SCANCODE_TO_KEYCODE(internal::BB_INTERNAL_SCANCODE_ENDCALL)
    };
}
