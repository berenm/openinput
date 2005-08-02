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
  int i;

  // Fill all with the unknown key
  for(i=SK_FIRST; i<SK_LAST; i++) {
    kn[i] = "key_unknown";
  }

  // Regulars
  kn[SK_BACKSPACE] =    "key_backspace";
  kn[SK_TAB] =          "key_tab";
  kn[SK_CLEAR] =        "key_clear";
  kn[SK_RETURN] =       "key_return";
  kn[SK_PAUSE] =        "key_pause";
  kn[SK_ESC] =          "key_escape";
  kn[SK_SPACE] =        "key_space";
  kn[SK_EXCLAIM] =      "key_exclaim";
  kn[SK_QUOTEDBL] =     "key_dblquote";
  kn[SK_HASH] =         "key_hash";
  kn[SK_DOLLAR] =       "key_dollar";
  kn[SK_AMPERSAND] =    "key_and";
  kn[SK_QUOTE] =        "key_quote";
  kn[SK_LEFTPAREN] =    "key_leftparan";
  kn[SK_RIGHTPAREN] =   "key_rightparan";
  kn[SK_ASTERISK] =     "key_asterisk";
  kn[SK_PLUS] =         "key_plus";
  kn[SK_COMMA] =        "key_comma";
  kn[SK_MINUS] =        "key_minus";
  kn[SK_PERIOD] =       "key_period";
  kn[SK_SLASH] =        "key_slash";
  kn[SK_0] =            "key_0";
  kn[SK_1] =            "key_1";
  kn[SK_2] =            "key_2";
  kn[SK_3] =            "key_3";
  kn[SK_4] =            "key_4";
  kn[SK_5] =            "key_5";
  kn[SK_6] =            "key_6";
  kn[SK_7] =            "key_7";
  kn[SK_8] =            "key_8";
  kn[SK_9] =            "key_9";
  kn[SK_COLON] =        "key_colon";
  kn[SK_SEMICOLON] =    "key_semicolon";
  kn[SK_LESS] =         "key_less";
  kn[SK_EQUALS] =       "key_equal";
  kn[SK_GREATER] =      "key_greater";
  kn[SK_QUESTION] =     "key_question";
  kn[SK_AT] =           "key_at";
  kn[SK_LEFTBRACKET] =  "key_leftbracket";
  kn[SK_BACKSLASH] =    "key_backslash";
  kn[SK_RIGHTBRACKET] = "key_rightbracket";
  kn[SK_CARET] =        "key_caret";
  kn[SK_UNDERSCORE] =   "key_underscore";
  kn[SK_BACKQUOTE] =    "key_backquote";
  kn[SK_A] =            "key_a";
  kn[SK_B] =            "key_b";
  kn[SK_C] =            "key_c";
  kn[SK_D] =            "key_d";
  kn[SK_E] =            "key_e";
  kn[SK_F] =            "key_f";
  kn[SK_G] =            "key_g";
  kn[SK_H] =            "key_h";
  kn[SK_I] =            "key_i";
  kn[SK_J] =            "key_j";
  kn[SK_K] =            "key_k";
  kn[SK_L] =            "key_l";
  kn[SK_M] =            "key_m";
  kn[SK_N] =            "key_n";
  kn[SK_O] =            "key_o";
  kn[SK_P] =            "key_p";
  kn[SK_Q] =            "key_q";
  kn[SK_R] =            "key_r";
  kn[SK_S] =            "key_s";
  kn[SK_T] =            "key_t";
  kn[SK_U] =            "key_u";
  kn[SK_V] =            "key_v";
  kn[SK_W] =            "key_w";
  kn[SK_X] =            "key_x";
  kn[SK_Y] =            "key_y";
  kn[SK_Z] =            "key_z";
  kn[SK_DELETE] =       "key_delete";

  // International
  kn[SK_INT_0] =        "key_int0";
  kn[SK_INT_1] =        "key_int1";
  kn[SK_INT_2] =        "key_int2";
  kn[SK_INT_3] =        "key_int3";
  kn[SK_INT_4] =        "key_int4";
  kn[SK_INT_5] =        "key_int5";
  kn[SK_INT_6] =        "key_int6";
  kn[SK_INT_7] =        "key_int7";
  kn[SK_INT_8] =        "key_int8";
  kn[SK_INT_9] =        "key_int9";
  kn[SK_INT_10] =       "key_int10";
  kn[SK_INT_11] =       "key_int11";
  kn[SK_INT_12] =       "key_int12";
  kn[SK_INT_13] =       "key_int13";
  kn[SK_INT_14] =       "key_int14";
  kn[SK_INT_15] =       "key_int15";
  kn[SK_INT_16] =       "key_int16";
  kn[SK_INT_17] =       "key_int17";
  kn[SK_INT_18] =       "key_int18";
  kn[SK_INT_19] =       "key_int19";
  kn[SK_INT_20] =       "key_int20";
  kn[SK_INT_21] =       "key_int21";
  kn[SK_INT_22] =       "key_int22";
  kn[SK_INT_23] =       "key_int23";
  kn[SK_INT_24] =       "key_int24";
  kn[SK_INT_25] =       "key_int25";
  kn[SK_INT_26] =       "key_int26";
  kn[SK_INT_27] =       "key_int27";
  kn[SK_INT_28] =       "key_int28";
  kn[SK_INT_29] =       "key_int29";
  kn[SK_INT_30] =       "key_int30";
  kn[SK_INT_31] =       "key_int31";
  kn[SK_INT_32] =       "key_int32";
  kn[SK_INT_33] =       "key_int33";
  kn[SK_INT_34] =       "key_int34";
  kn[SK_INT_35] =       "key_int35";
  kn[SK_INT_36] =       "key_int36";
  kn[SK_INT_37] =       "key_int37";
  kn[SK_INT_38] =       "key_int38";
  kn[SK_INT_39] =       "key_int39";
  kn[SK_INT_40] =       "key_int40";
  kn[SK_INT_41] =       "key_int41";
  kn[SK_INT_42] =       "key_int42";
  kn[SK_INT_43] =       "key_int43";
  kn[SK_INT_44] =       "key_int44";
  kn[SK_INT_45] =       "key_int45";
  kn[SK_INT_46] =       "key_int46";
  kn[SK_INT_47] =       "key_int47";
  kn[SK_INT_48] =       "key_int48";
  kn[SK_INT_49] =       "key_int49";
  kn[SK_INT_50] =       "key_int50";
  kn[SK_INT_51] =       "key_int51";
  kn[SK_INT_52] =       "key_int52";
  kn[SK_INT_53] =       "key_int53";
  kn[SK_INT_54] =       "key_int54";
  kn[SK_INT_55] =       "key_int55";
  kn[SK_INT_56] =       "key_int56";
  kn[SK_INT_57] =       "key_int57";
  kn[SK_INT_58] =       "key_int58";
  kn[SK_INT_59] =       "key_int59";
  kn[SK_INT_60] =       "key_int60";
  kn[SK_INT_61] =       "key_int61";
  kn[SK_INT_62] =       "key_int62";
  kn[SK_INT_63] =       "key_int63";
  kn[SK_INT_64] =       "key_int64";
  kn[SK_INT_65] =       "key_int65";
  kn[SK_INT_66] =       "key_int66";
  kn[SK_INT_67] =       "key_int67";
  kn[SK_INT_68] =       "key_int68";
  kn[SK_INT_69] =       "key_int69";
  kn[SK_INT_70] =       "key_int70";
  kn[SK_INT_71] =       "key_int71";
  kn[SK_INT_72] =       "key_int72";
  kn[SK_INT_73] =       "key_int73";
  kn[SK_INT_74] =       "key_int74";
  kn[SK_INT_75] =       "key_int75";
  kn[SK_INT_76] =       "key_int76";
  kn[SK_INT_77] =       "key_int77";
  kn[SK_INT_78] =       "key_int78";
  kn[SK_INT_79] =       "key_int79";
  kn[SK_INT_80] =       "key_int80";
  kn[SK_INT_81] =       "key_int81";
  kn[SK_INT_82] =       "key_int82";
  kn[SK_INT_83] =       "key_int83";
  kn[SK_INT_84] =       "key_int84";
  kn[SK_INT_85] =       "key_int85";
  kn[SK_INT_86] =       "key_int86";
  kn[SK_INT_87] =       "key_int87";
  kn[SK_INT_88] =       "key_int88";
  kn[SK_INT_89] =       "key_int89";
  kn[SK_INT_90] =       "key_int90";
  kn[SK_INT_91] =       "key_int91";
  kn[SK_INT_92] =       "key_int92";
  kn[SK_INT_93] =       "key_int93";
  kn[SK_INT_94] =       "key_int94";
  kn[SK_INT_95] =       "key_int95";
  
  // Numeric keypad
  kn[SK_N_0] =          "key_num_0";
  kn[SK_N_1] =          "key_num_1";
  kn[SK_N_2] =          "key_num_2";
  kn[SK_N_3] =          "key_num_3";
  kn[SK_N_4] =          "key_num_4";
  kn[SK_N_5] =          "key_num_5";
  kn[SK_N_6] =          "key_num_6";
  kn[SK_N_7] =          "key_num_7";
  kn[SK_N_8] =          "key_num_8";
  kn[SK_N_9] =          "key_num_9";
  kn[SK_N_PERIOD] =     "key_num_period";
  kn[SK_N_DIVIDE] =     "key_num_divide";
  kn[SK_N_MULTIPLY] =   "key_num_multiply";
  kn[SK_N_MINUS] =      "key_num_minus";
  kn[SK_N_PLUS] =       "key_num_plus";
  kn[SK_N_ENTER] =      "key_num_enter";
  kn[SK_N_EQUALS] =     "key_num_equals";
  
  // Arrows + home/end pad
  kn[SK_UP] =           "key_up";
  kn[SK_DOWN] =         "key_down";
  kn[SK_RIGHT] =        "key_right";
  kn[SK_LEFT] =         "key_left";
  kn[SK_INSERT] =       "key_insert";
  kn[SK_HOME] =         "key_home";
  kn[SK_END] =          "key_end";
  kn[SK_PAGEUP] =       "key_pageup";
  kn[SK_PAGEDOWN] =     "key_pagedown";
  
  // Function keys
  kn[SK_F1] =           "key_f1";
  kn[SK_F2] =           "key_f2";
  kn[SK_F3] =           "key_f3";
  kn[SK_F4] =           "key_f4";
  kn[SK_F5] =           "key_f5";
  kn[SK_F6] =           "key_f6";
  kn[SK_F7] =           "key_f7";
  kn[SK_F8] =           "key_f8";
  kn[SK_F9] =           "key_f9";
  kn[SK_F10] =          "key_f10";
  kn[SK_F11] =          "key_f11";
  kn[SK_F12] =          "key_f12";
  kn[SK_F13] =          "key_f13";
  kn[SK_F14] =          "key_f14";
  kn[SK_F15] =          "key_f15";
  
  // Key state modifiers
  kn[SK_NUMLOCK] =      "key_numlock";
  kn[SK_CAPSLOCK] =     "key_capslock";
  kn[SK_SCROLLOCK] =    "key_scrolllock";
  kn[SK_RSHIFT] =       "key_shift_right";
  kn[SK_LSHIFT] =       "key_shift_left";
  kn[SK_RCTRL] =        "key_control_right";
  kn[SK_LCTRL] =        "key_control_left";
  kn[SK_RALT] =         "key_alt_right";
  kn[SK_LALT] =         "key_alt_left";
  kn[SK_RMETA] =        "key_meta_right";
  kn[SK_LMETA] =        "key_meta_left";
  kn[SK_RWINDOWS] =     "key_windows_right";
  kn[SK_LWINDOWS] =     "key_windows_left";
  kn[SK_ALTGR] =        "key_altgr";
  kn[SK_COMPOSE] =      "key_compose";
  
  // Miscellaneous function keys
  kn[SK_HELP] =         "key_help";
  kn[SK_PRINT] =        "key_print";
  kn[SK_SYSREQ] =       "key_sysrq";
  kn[SK_BREAK] =        "key_break";
  kn[SK_MENU] =         "key_menu";
  kn[SK_POWER] =        "key_power";
  kn[SK_EURO] =         "key_euro";
  kn[SK_UNDO] =         "key_undo";

  return SINP_ERR_OK;
}

/* ******************************************************************** */
