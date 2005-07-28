/*
 * keynames.c : Keyboard name definitions
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

// Includes
#include "config.h"
#include "sinp.h"
#include "internal.h"

/* ******************************************************************** */

// Fill keyboard names
sint keyboard_fillnames(char **kn) {

  // Regulars
  kn[SK_BACKSPACE] =    "backspace";
  kn[SK_TAB] =          "tab";
  kn[SK_CLEAR] =        "clear";
  kn[SK_RETURN] =       "return";
  kn[SK_PAUSE] =        "pause";
  kn[SK_ESC] =          "escape";
  kn[SK_SPACE] =        "space";
  kn[SK_EXCLAIM] =      "exclaim";
  kn[SK_QUOTEDBL] =     "dblquote";
  kn[SK_HASH] =         "hash";
  kn[SK_DOLLAR] =       "dollar";
  kn[SK_AMPERSAND] =    "and";
  kn[SK_QUOTE] =        "quote";
  kn[SK_LEFTPAREN] =    "leftparan";
  kn[SK_RIGHTPAREN] =   "rightparan";
  kn[SK_ASTERISK] =     "asterisk";
  kn[SK_PLUS] =         "plus";
  kn[SK_COMMA] =        "comma";
  kn[SK_MINUS] =        "minus";
  kn[SK_PERIOD] =       "period";
  kn[SK_SLASH] =        "slash";
  kn[SK_0] =            "0";
  kn[SK_1] =            "1";
  kn[SK_2] =            "2";
  kn[SK_3] =            "3";
  kn[SK_4] =            "4";
  kn[SK_5] =            "5";
  kn[SK_6] =            "6";
  kn[SK_7] =            "7";
  kn[SK_8] =            "8";
  kn[SK_9] =            "9";
  kn[SK_COLON] =        "colon";
  kn[SK_SEMICOLON] =    "semicolon";
  kn[SK_LESS] =         "less";
  kn[SK_EQUALS] =       "equal";
  kn[SK_GREATER] =      "greater";
  kn[SK_QUESTION] =     "question";
  kn[SK_AT] =           "at";
  kn[SK_LEFTBRACKET] =  "leftbracket";
  kn[SK_BACKSLASH] =    "backslash";
  kn[SK_RIGHTBRACKET] = "rightbracket";
  kn[SK_CARET] =        "caret";
  kn[SK_UNDERSCORE] =   "underscore";
  kn[SK_BACKQUOTE] =    "backquote";
  kn[SK_A] =            "a";
  kn[SK_B] =            "b";
  kn[SK_C] =            "c";
  kn[SK_D] =            "d";
  kn[SK_E] =            "e";
  kn[SK_F] =            "f";
  kn[SK_G] =            "g";
  kn[SK_H] =            "h";
  kn[SK_I] =            "i";
  kn[SK_J] =            "j";
  kn[SK_K] =            "k";
  kn[SK_L] =            "l";
  kn[SK_M] =            "m";
  kn[SK_N] =            "n";
  kn[SK_O] =            "o";
  kn[SK_P] =            "p";
  kn[SK_Q] =            "q";
  kn[SK_R] =            "r";
  kn[SK_S] =            "s";
  kn[SK_T] =            "t";
  kn[SK_U] =            "u";
  kn[SK_V] =            "v";
  kn[SK_W] =            "x";
  kn[SK_X] =            "x";
  kn[SK_Y] =            "y";
  kn[SK_Z] =            "z";
  kn[SK_DELETE] =       "delete";

  // International
  kn[SK_INT_0] =        "int0";
  kn[SK_INT_1] =        "int1";
  kn[SK_INT_2] =        "int2";
  kn[SK_INT_3] =        "int3";
  kn[SK_INT_4] =        "int4";
  kn[SK_INT_5] =        "int5";
  kn[SK_INT_6] =        "int6";
  kn[SK_INT_7] =        "int7";
  kn[SK_INT_8] =        "int8";
  kn[SK_INT_9] =        "int9";
  kn[SK_INT_10] =       "int10";
  kn[SK_INT_11] =       "int11";
  kn[SK_INT_12] =       "int12";
  kn[SK_INT_13] =       "int13";
  kn[SK_INT_14] =       "int14";
  kn[SK_INT_15] =       "int15";
  kn[SK_INT_16] =       "int16";
  kn[SK_INT_17] =       "int17";
  kn[SK_INT_18] =       "int18";
  kn[SK_INT_19] =       "int19";
  kn[SK_INT_20] =       "int20";
  kn[SK_INT_21] =       "int21";
  kn[SK_INT_22] =       "int22";
  kn[SK_INT_23] =       "int23";
  kn[SK_INT_24] =       "int24";
  kn[SK_INT_25] =       "int25";
  kn[SK_INT_26] =       "int26";
  kn[SK_INT_27] =       "int27";
  kn[SK_INT_28] =       "int28";
  kn[SK_INT_29] =       "int29";
  kn[SK_INT_30] =       "int30";
  kn[SK_INT_31] =       "int31";
  kn[SK_INT_32] =       "int32";
  kn[SK_INT_33] =       "int33";
  kn[SK_INT_34] =       "int34";
  kn[SK_INT_35] =       "int35";
  kn[SK_INT_36] =       "int36";
  kn[SK_INT_37] =       "int37";
  kn[SK_INT_38] =       "int38";
  kn[SK_INT_39] =       "int39";
  kn[SK_INT_40] =       "int40";
  kn[SK_INT_41] =       "int41";
  kn[SK_INT_42] =       "int42";
  kn[SK_INT_43] =       "int43";
  kn[SK_INT_44] =       "int44";
  kn[SK_INT_45] =       "int45";
  kn[SK_INT_46] =       "int46";
  kn[SK_INT_47] =       "int47";
  kn[SK_INT_48] =       "int48";
  kn[SK_INT_49] =       "int49";
  kn[SK_INT_50] =       "int50";
  kn[SK_INT_51] =       "int51";
  kn[SK_INT_52] =       "int52";
  kn[SK_INT_53] =       "int53";
  kn[SK_INT_54] =       "int54";
  kn[SK_INT_55] =       "int55";
  kn[SK_INT_56] =       "int56";
  kn[SK_INT_57] =       "int57";
  kn[SK_INT_58] =       "int58";
  kn[SK_INT_59] =       "int59";
  kn[SK_INT_60] =       "int60";
  kn[SK_INT_61] =       "int61";
  kn[SK_INT_62] =       "int62";
  kn[SK_INT_63] =       "int63";
  kn[SK_INT_64] =       "int64";
  kn[SK_INT_65] =       "int65";
  kn[SK_INT_66] =       "int66";
  kn[SK_INT_67] =       "int67";
  kn[SK_INT_68] =       "int68";
  kn[SK_INT_69] =       "int69";
  kn[SK_INT_70] =       "int70";
  kn[SK_INT_71] =       "int71";
  kn[SK_INT_72] =       "int72";
  kn[SK_INT_73] =       "int73";
  kn[SK_INT_74] =       "int74";
  kn[SK_INT_75] =       "int75";
  kn[SK_INT_76] =       "int76";
  kn[SK_INT_77] =       "int77";
  kn[SK_INT_78] =       "int78";
  kn[SK_INT_79] =       "int79";
  kn[SK_INT_80] =       "int80";
  kn[SK_INT_81] =       "int81";
  kn[SK_INT_82] =       "int82";
  kn[SK_INT_83] =       "int83";
  kn[SK_INT_84] =       "int84";
  kn[SK_INT_85] =       "int85";
  kn[SK_INT_86] =       "int86";
  kn[SK_INT_87] =       "int87";
  kn[SK_INT_88] =       "int88";
  kn[SK_INT_89] =       "int89";
  kn[SK_INT_90] =       "int90";
  kn[SK_INT_91] =       "int91";
  kn[SK_INT_92] =       "int92";
  kn[SK_INT_93] =       "int93";
  kn[SK_INT_94] =       "int94";
  kn[SK_INT_95] =       "int95";
  
  // Numeric keypad
  kn[SK_N_0] =          "num_0";
  kn[SK_N_1] =          "num_1";
  kn[SK_N_2] =          "num_2";
  kn[SK_N_3] =          "num_3";
  kn[SK_N_4] =          "num_4";
  kn[SK_N_5] =          "num_5";
  kn[SK_N_6] =          "num_6";
  kn[SK_N_7] =          "num_7";
  kn[SK_N_8] =          "num_8";
  kn[SK_N_9] =          "num_9";
  kn[SK_N_PERIOD] =     "num_period";
  kn[SK_N_DIVIDE] =     "num_divide";
  kn[SK_N_MULTIPLY] =   "num_multiply";
  kn[SK_N_MINUS] =      "num_minus";
  kn[SK_N_PLUS] =       "num_plus";
  kn[SK_N_ENTER] =      "num_enter";
  kn[SK_N_EQUALS] =     "num_equals";
  
  // Arrows + home/end pad
  kn[SK_UP] =           "up";
  kn[SK_DOWN] =         "down";
  kn[SK_RIGHT] =        "right";
  kn[SK_LEFT] =         "left";
  kn[SK_INSERT] =       "insert";
  kn[SK_HOME] =         "home";
  kn[SK_END] =          "end";
  kn[SK_PAGEUP] =       "pageup";
  kn[SK_PAGEDOWN] =     "pagedown";
  
  // Function keys
  kn[SK_F1] =           "f1";
  kn[SK_F2] =           "f2";
  kn[SK_F3] =           "f3";
  kn[SK_F4] =           "f4";
  kn[SK_F5] =           "f5";
  kn[SK_F6] =           "f6";
  kn[SK_F7] =           "f7";
  kn[SK_F8] =           "f8";
  kn[SK_F9] =           "f9";
  kn[SK_F10] =          "f10";
  kn[SK_F11] =          "f11";
  kn[SK_F12] =          "f12";
  kn[SK_F13] =          "f13";
  kn[SK_F14] =          "f14";
  kn[SK_F15] =          "f15";
  
  // Key state modifiers
  kn[SK_NUMLOCK] =      "numlock";
  kn[SK_CAPSLOCK] =     "capslock";
  kn[SK_SCROLLOCK] =    "scrolllock";
  kn[SK_RSHIFT] =       "shift_right";
  kn[SK_LSHIFT] =       "shift_left";
  kn[SK_RCTRL] =        "control_right";
  kn[SK_LCTRL] =        "control_left";
  kn[SK_RALT] =         "alt_right";
  kn[SK_LALT] =         "alt_left";
  kn[SK_RMETA] =        "meta_right";
  kn[SK_LMETA] =        "meta_left";
  kn[SK_RWINDOWS] =     "windows_right";
  kn[SK_LWINDOWS] =     "windows_left";
  kn[SK_ALTGR] =        "altgr";
  kn[SK_COMPOSE] =      "compose";
  
  // Miscellaneous function keys
  kn[SK_HELP] =         "help";
  kn[SK_PRINT] =        "print";
  kn[SK_SYSREQ] =       "sysrq";
  kn[SK_BREAK] =        "break";
  kn[SK_MENU] =         "menu";
  kn[SK_POWER] =        "power";
  kn[SK_EURO] =         "euro";
  kn[SK_UNDO] =         "undo";

  return SINP_ERR_OK;
}

/* ******************************************************************** */
