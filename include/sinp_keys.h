/*
 * sinp_keys.h : Basic key and modifier definitions
 *
 * This file is a part of libsinp - the simple input library.
 * Copyright (C) 2005  Jakob Kjaer <makob@makob.dk>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ******************************************************************** */

#ifndef _SINP_KEYS_H_
#define _SINP_KEYS_H_

#ifndef _SINP_H_
#error Do not include this file directly - use sinp.h
#endif

/* ********************************************************************
 * Keyboard button definitions
 ******************************************************************** */

// Key definitions
typedef enum {
  SK_UNKNOWN		= 0,
  SK_FIRST		= 0,
  
  // Ascii-keys
  SK_BACKSPACE		= 8,
  SK_TAB		= 9,
  SK_CLEAR		= 12,
  SK_RETURN		= 13,
  SK_PAUSE		= 19,
  SK_ESCAPE		= 27,
  SK_SPACE		= 32,
  SK_EXCLAIM		= 33,
  SK_QUOTEDBL		= 34,
  SK_HASH		= 35,
  SK_DOLLAR		= 36,
  SK_AMPERSAND		= 38,
  SK_QUOTE		= 39,
  SK_LEFTPAREN		= 40,
  SK_RIGHTPAREN		= 41,
  SK_ASTERISK		= 42,
  SK_PLUS		= 43,
  SK_COMMA		= 44,
  SK_MINUS		= 45,
  SK_PERIOD		= 46,
  SK_SLASH		= 47,
  SK_0			= 48,
  SK_1			= 49,
  SK_2			= 50,
  SK_3			= 51,
  SK_4			= 52,
  SK_5			= 53,
  SK_6			= 54,
  SK_7			= 55,
  SK_8			= 56,
  SK_9			= 57,
  SK_COLON		= 58,
  SK_SEMICOLON		= 59,
  SK_LESS		= 60,
  SK_EQUALS		= 61,
  SK_GREATER		= 62,
  SK_QUESTION		= 63,
  SK_AT			= 64,
  // Here we skip uppercase letters
  SK_LEFTBRACKET	= 91,
  SK_BACKSLASH		= 92,
  SK_RIGHTBRACKET	= 93,
  SK_CARET		= 94,
  SK_UNDERSCORE		= 95,
  SK_BACKQUOTE		= 96,
  SK_A			= 97,
  SK_B			= 98,
  SK_C			= 99,
  SK_D			= 100,
  SK_E			= 101,
  SK_F			= 102,
  SK_G			= 103,
  SK_H			= 104,
  SK_I 			= 105,
  SK_J			= 106,
  SK_K			= 107,
  SK_L			= 108,
  SK_M			= 109,
  SK_N			= 110,
  SK_O			= 111,
  SK_P			= 112,
  SK_Q			= 113,
  SK_R			= 114,
  SK_S			= 115,
  SK_T 			= 116,
  SK_U			= 117,
  SK_V			= 118,
  SK_W			= 119,
  SK_X			= 120,
  SK_Y			= 121,
  SK_Z 			= 122,
  SK_DELETE		= 127,

  // International character (no more ascii)
  SK_INT_0		= 160, // 0xA0
  SK_INT_1		= 161,
  SK_INT_2		= 162,
  SK_INT_3		= 163,
  SK_INT_4		= 164,
  SK_INT_5		= 165,
  SK_INT_6		= 166,
  SK_INT_7		= 167,
  SK_INT_8		= 168,
  SK_INT_9		= 169,
  SK_INT_10		= 170,
  SK_INT_11		= 171,
  SK_INT_12		= 172,
  SK_INT_13		= 173,
  SK_INT_14		= 174,
  SK_INT_15		= 175,
  SK_INT_16		= 176,
  SK_INT_17		= 177,
  SK_INT_18		= 178,
  SK_INT_19		= 179,
  SK_INT_20		= 180,
  SK_INT_21		= 181,
  SK_INT_22		= 182,
  SK_INT_23		= 183,
  SK_INT_24		= 184,
  SK_INT_25		= 185,
  SK_INT_26		= 186,
  SK_INT_27		= 187,
  SK_INT_28		= 188,
  SK_INT_29		= 189,
  SK_INT_30		= 190,
  SK_INT_31		= 191,
  SK_INT_32		= 192,
  SK_INT_33		= 193,
  SK_INT_34		= 194,
  SK_INT_35		= 195,
  SK_INT_36		= 196,
  SK_INT_37		= 197,
  SK_INT_38		= 198,
  SK_INT_39		= 199,
  SK_INT_40		= 200,
  SK_INT_41		= 201,
  SK_INT_42		= 202,
  SK_INT_43		= 203,
  SK_INT_44		= 204,
  SK_INT_45		= 205,
  SK_INT_46		= 206,
  SK_INT_47		= 207,
  SK_INT_48		= 208,
  SK_INT_49		= 209,
  SK_INT_50		= 210,
  SK_INT_51		= 211,
  SK_INT_52		= 212,
  SK_INT_53		= 213,
  SK_INT_54		= 214,
  SK_INT_55		= 215,
  SK_INT_56		= 216,
  SK_INT_57		= 217,
  SK_INT_58		= 218,
  SK_INT_59		= 219,
  SK_INT_60		= 220,
  SK_INT_61		= 221,
  SK_INT_62		= 222,
  SK_INT_63		= 223,
  SK_INT_64		= 224,
  SK_INT_65		= 225,
  SK_INT_66		= 226,
  SK_INT_67		= 227,
  SK_INT_68		= 228,
  SK_INT_69		= 229,
  SK_INT_70		= 230,
  SK_INT_71		= 231,
  SK_INT_72		= 232,
  SK_INT_73		= 233,
  SK_INT_74		= 234,
  SK_INT_75		= 235,
  SK_INT_76		= 236,
  SK_INT_77		= 237,
  SK_INT_78		= 238,
  SK_INT_79		= 239,
  SK_INT_80		= 240,
  SK_INT_81		= 241,
  SK_INT_82		= 242,
  SK_INT_83		= 243,
  SK_INT_84		= 244,
  SK_INT_85		= 245,
  SK_INT_86		= 246,
  SK_INT_87		= 247,
  SK_INT_88		= 248,
  SK_INT_89		= 249,
  SK_INT_90		= 250,
  SK_INT_91		= 251,
  SK_INT_92		= 252,
  SK_INT_93		= 253,
  SK_INT_94		= 254,
  SK_INT_95		= 255, // 0xFF

  // Numeric keypad
  SK_N_0		= 256,
  SK_N_1		= 257,
  SK_N_2		= 258,
  SK_N_3		= 259,
  SK_N_4		= 260,
  SK_N_5		= 261,
  SK_N_6		= 262,
  SK_N_7		= 263,
  SK_N_8		= 264,
  SK_N_9		= 265,
  SK_N_PERIOD		= 266,
  SK_N_DIVIDE		= 267,
  SK_N_MULTIPLY         = 268,
  SK_N_MINUS		= 269,
  SK_N_PLUS		= 270,
  SK_N_ENTER		= 271,
  SK_N_EQUALS		= 272,
  
  // Arrows + home/end pad
  SK_UP			= 273,
  SK_DOWN		= 274,
  SK_RIGHT		= 275,
  SK_LEFT		= 276,
  SK_INSERT		= 277,
  SK_HOME		= 278,
  SK_END		= 279,
  SK_PAGEUP		= 280,
  SK_PAGEDOWN		= 281,
  
  // Function keys
  SK_F1			= 282,
  SK_F2			= 283,
  SK_F3			= 284,
  SK_F4			= 285,
  SK_F5			= 286,
  SK_F6			= 287,
  SK_F7			= 288,
  SK_F8			= 289,
  SK_F9			= 290,
  SK_F10		= 291,
  SK_F11		= 292,
  SK_F12		= 293,
  SK_F13		= 294,
  SK_F14		= 295,
  SK_F15		= 296,
  
  // Key state modifiers
  SK_NUMLOCK		= 300,
  SK_CAPSLOCK		= 301,
  SK_SCROLLOCK		= 302,
  SK_RSHIFT		= 303,
  SK_LSHIFT		= 304,
  SK_RCTRL		= 305,
  SK_LCTRL		= 306,
  SK_RALT		= 307,
  SK_LALT		= 308,
  SK_RMETA		= 309,
  SK_LMETA		= 310,
  SK_LSUPER		= 311, // Left Windows key
  SK_RSUPER		= 312, // Right Windows key
  SK_MODE		= 313, // Alt-Gr key
  SK_COMPOSE		= 314, // Multi-key compose key
  
  // Miscellaneous function keys
  SK_HELP		= 315,
  SK_PRINT		= 316,
  SK_SYSREQ		= 317,
  SK_BREAK		= 318,
  SK_MENU		= 319,
  SK_POWER		= 320, // Power Macintosh power key
  SK_EURO		= 321, // Some european keyboards
  SK_UNDO		= 322, // Atari keyboard has Undo

  SK_LAST
} sinp_key;

/* ********************************************************************
 * Modifier button definitions
 ******************************************************************** */

// Modifier definitions
typedef enum {
  SM_NONE               = 0x0000,
  SM_LSHIFT             = 0x0001,
  SM_RSHIFT             = 0x0002,
  SM_LCTRL              = 0x0040,
  SM_RCTRL              = 0x0080,
  SM_LALT               = 0x0100,
  SM_RALT               = 0x0200,
  SM_LMETA              = 0x0400,
  SM_RMETA              = 0x0800,
  SM_NUM                = 0x1000,
  SM_CAPS               = 0x2000,
  SM_MODE               = 0x4000,
  SM_RESERVED           = 0x8000
} sinp_mod;

/* ********************************************************************
 * Key symbol structure
 ******************************************************************** */

// Keyboard virtual symbol
typedef struct {
  uchar scancode;       // Hardware scancode
  sinp_key sym;         // Key symbol
  sinp_mod mod;         // Modifier symbol
  ushort unicode;       // Translated unicode character
} sinp_keysym;
  
/* ******************************************************************** */

#endif
