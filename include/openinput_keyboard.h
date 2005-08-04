/*
 * openinput_keys.h : Basic key and modifier definitions
 *
 * This file is a part of the OpenInput library.
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

#ifndef _OPENINPUT_KEYS_H_
#define _OPENINPUT_KEYS_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ********************************************************************
 * Keyboard button definitions
 ******************************************************************** */

// Key definitions
typedef enum {
  OIK_UNKNOWN		= 0,
  OIK_FIRST		= 0,
  
  // Ascii-keys
  OIK_BACKSPACE		= 8,
  OIK_TAB		= 9,
  OIK_CLEAR		= 12,
  OIK_RETURN		= 13,
  OIK_PAUSE		= 19,
  OIK_ESC	        = 27,
  OIK_SPACE		= 32,
  OIK_EXCLAIM		= 33,
  OIK_QUOTEDBL		= 34,
  OIK_HASH		= 35,
  OIK_DOLLAR		= 36,
  OIK_AMPERSAND		= 38,
  OIK_QUOTE		= 39,
  OIK_LEFTPAREN		= 40,
  OIK_RIGHTPAREN		= 41,
  OIK_ASTERISK		= 42,
  OIK_PLUS		= 43,
  OIK_COMMA		= 44,
  OIK_MINUS		= 45,
  OIK_PERIOD		= 46,
  OIK_SLASH		= 47,
  OIK_0			= 48,
  OIK_1			= 49,
  OIK_2			= 50,
  OIK_3			= 51,
  OIK_4			= 52,
  OIK_5			= 53,
  OIK_6			= 54,
  OIK_7			= 55,
  OIK_8			= 56,
  OIK_9			= 57,
  OIK_COLON		= 58,
  OIK_SEMICOLON		= 59,
  OIK_LESS		= 60,
  OIK_EQUALS		= 61,
  OIK_GREATER		= 62,
  OIK_QUESTION		= 63,
  OIK_AT			= 64,
  // Here we skip uppercase letters
  OIK_LEFTBRACKET	= 91,
  OIK_BACKSLASH		= 92,
  OIK_RIGHTBRACKET	= 93,
  OIK_CARET		= 94,
  OIK_UNDERSCORE		= 95,
  OIK_BACKQUOTE		= 96,
  OIK_A			= 97,
  OIK_B			= 98,
  OIK_C			= 99,
  OIK_D			= 100,
  OIK_E			= 101,
  OIK_F			= 102,
  OIK_G			= 103,
  OIK_H			= 104,
  OIK_I 			= 105,
  OIK_J			= 106,
  OIK_K			= 107,
  OIK_L			= 108,
  OIK_M			= 109,
  OIK_N			= 110,
  OIK_O			= 111,
  OIK_P			= 112,
  OIK_Q			= 113,
  OIK_R			= 114,
  OIK_S			= 115,
  OIK_T 			= 116,
  OIK_U			= 117,
  OIK_V			= 118,
  OIK_W			= 119,
  OIK_X			= 120,
  OIK_Y			= 121,
  OIK_Z 			= 122,
  OIK_DELETE		= 127,

  // International character (no more ascii)
  OIK_INT_0		= 160, // 0xA0
  OIK_INT_1		= 161,
  OIK_INT_2		= 162,
  OIK_INT_3		= 163,
  OIK_INT_4		= 164,
  OIK_INT_5		= 165,
  OIK_INT_6		= 166,
  OIK_INT_7		= 167,
  OIK_INT_8		= 168,
  OIK_INT_9		= 169,
  OIK_INT_10		= 170,
  OIK_INT_11		= 171,
  OIK_INT_12		= 172,
  OIK_INT_13		= 173,
  OIK_INT_14		= 174,
  OIK_INT_15		= 175,
  OIK_INT_16		= 176,
  OIK_INT_17		= 177,
  OIK_INT_18		= 178,
  OIK_INT_19		= 179,
  OIK_INT_20		= 180,
  OIK_INT_21		= 181,
  OIK_INT_22		= 182,
  OIK_INT_23		= 183,
  OIK_INT_24		= 184,
  OIK_INT_25		= 185,
  OIK_INT_26		= 186,
  OIK_INT_27		= 187,
  OIK_INT_28		= 188,
  OIK_INT_29		= 189,
  OIK_INT_30		= 190,
  OIK_INT_31		= 191,
  OIK_INT_32		= 192,
  OIK_INT_33		= 193,
  OIK_INT_34		= 194,
  OIK_INT_35		= 195,
  OIK_INT_36		= 196,
  OIK_INT_37		= 197,
  OIK_INT_38		= 198,
  OIK_INT_39		= 199,
  OIK_INT_40		= 200,
  OIK_INT_41		= 201,
  OIK_INT_42		= 202,
  OIK_INT_43		= 203,
  OIK_INT_44		= 204,
  OIK_INT_45		= 205,
  OIK_INT_46		= 206,
  OIK_INT_47		= 207,
  OIK_INT_48		= 208,
  OIK_INT_49		= 209,
  OIK_INT_50		= 210,
  OIK_INT_51		= 211,
  OIK_INT_52		= 212,
  OIK_INT_53		= 213,
  OIK_INT_54		= 214,
  OIK_INT_55		= 215,
  OIK_INT_56		= 216,
  OIK_INT_57		= 217,
  OIK_INT_58		= 218,
  OIK_INT_59		= 219,
  OIK_INT_60		= 220,
  OIK_INT_61		= 221,
  OIK_INT_62		= 222,
  OIK_INT_63		= 223,
  OIK_INT_64		= 224,
  OIK_INT_65		= 225,
  OIK_INT_66		= 226,
  OIK_INT_67		= 227,
  OIK_INT_68		= 228,
  OIK_INT_69		= 229,
  OIK_INT_70		= 230,
  OIK_INT_71		= 231,
  OIK_INT_72		= 232,
  OIK_INT_73		= 233,
  OIK_INT_74		= 234,
  OIK_INT_75		= 235,
  OIK_INT_76		= 236,
  OIK_INT_77		= 237,
  OIK_INT_78		= 238,
  OIK_INT_79		= 239,
  OIK_INT_80		= 240,
  OIK_INT_81		= 241,
  OIK_INT_82		= 242,
  OIK_INT_83		= 243,
  OIK_INT_84		= 244,
  OIK_INT_85		= 245,
  OIK_INT_86		= 246,
  OIK_INT_87		= 247,
  OIK_INT_88		= 248,
  OIK_INT_89		= 249,
  OIK_INT_90		= 250,
  OIK_INT_91		= 251,
  OIK_INT_92		= 252,
  OIK_INT_93		= 253,
  OIK_INT_94		= 254,
  OIK_INT_95		= 255, // 0xFF

  // Numeric keypad
  OIK_N_0		= 256,
  OIK_N_1		= 257,
  OIK_N_2		= 258,
  OIK_N_3		= 259,
  OIK_N_4		= 260,
  OIK_N_5		= 261,
  OIK_N_6		= 262,
  OIK_N_7		= 263,
  OIK_N_8		= 264,
  OIK_N_9		= 265,
  OIK_N_PERIOD		= 266,
  OIK_N_DIVIDE		= 267,
  OIK_N_MULTIPLY         = 268,
  OIK_N_MINUS		= 269,
  OIK_N_PLUS		= 270,
  OIK_N_ENTER		= 271,
  OIK_N_EQUALS		= 272,
  
  // Arrows + home/end pad
  OIK_UP			= 273,
  OIK_DOWN		= 274,
  OIK_RIGHT		= 275,
  OIK_LEFT		= 276,
  OIK_INSERT		= 277,
  OIK_HOME		= 278,
  OIK_END		= 279,
  OIK_PAGEUP		= 280,
  OIK_PAGEDOWN		= 281,
  
  // Function keys
  OIK_F1			= 282,
  OIK_F2			= 283,
  OIK_F3			= 284,
  OIK_F4			= 285,
  OIK_F5			= 286,
  OIK_F6			= 287,
  OIK_F7			= 288,
  OIK_F8			= 289,
  OIK_F9			= 290,
  OIK_F10		= 291,
  OIK_F11		= 292,
  OIK_F12		= 293,
  OIK_F13		= 294,
  OIK_F14		= 295,
  OIK_F15		= 296,
  
  // Key state modifiers
  OIK_NUMLOCK		= 300,
  OIK_CAPSLOCK		= 301,
  OIK_SCROLLOCK		= 302,
  OIK_RSHIFT		= 303,
  OIK_LSHIFT		= 304,
  OIK_RCTRL		= 305,
  OIK_LCTRL		= 306,
  OIK_RALT		= 307,
  OIK_LALT		= 308,
  OIK_RMETA		= 309,
  OIK_LMETA		= 310,
  OIK_RWINDOWS    	= 311,
  OIK_LWINDOWS		= 312,
  OIK_ALTGR		= 313,
  OIK_COMPOSE		= 314, // Multi-key compose key
  
  // Miscellaneous function keys
  OIK_HELP		= 315,
  OIK_PRINT		= 316,
  OIK_SYSREQ		= 317,
  OIK_BREAK		= 318,
  OIK_MENU		= 319, // Windows "menu" key
  OIK_POWER		= 320, // Power Macintosh power key
  OIK_EURO		= 321, // Some european keyboards
  OIK_UNDO		= 322, // Atari keyboard has Undo

  OIK_LAST
} oi_key;

/* ********************************************************************
 * Modifier button definitions
 ******************************************************************** */

// Modifier definitions
#define OIM_NONE         0x0000
#define OIM_LSHIFT       0x0001
#define OIM_RSHIFT       0x0002
#define OIM_LCTRL        0x0040
#define OIM_RCTRL        0x0080
#define OIM_LALT         0x0100
#define OIM_RALT         0x0200
#define OIM_LMETA        0x0400
#define OIM_RMETA        0x0800
#define OIM_NUMLOCK      0x1000
#define OIM_CAPSLOCK     0x2000
#define OIM_ALTGR        0x4000
#define OIM_SCROLLOCK    0x8000

/* ********************************************************************
 * Key symbol structure
 ******************************************************************** */

// Keyboard virtual symbol
typedef struct {
  uchar scancode;       // Hardware scancode
  oi_key sym;         // Key symbol
  uint mod;             // Modifier symbol
} oi_keysym;
  
/* ******************************************************************** */

#endif
