/*
 * x11_translate.c : Translate X11 events to SINP events
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "internal.h"
#include "x11.h"

// Globals
static sinp_key x11_oddmap[256];
static sinp_key x11_miscmap[256];

/* ******************************************************************** */

// Initialize X11 -> sinp keymapper
void x11_initkeymap() {
  int i;

  // Clear odd and misc keymaps
  for(i=0; i<(sizeof(x11_oddmap)/sizeof(x11_oddmap[0])); i++) {
    x11_oddmap[i] = SK_UNKNOWN;
    x11_miscmap[i] = SK_UNKNOWN;
  }

  // Odd keymap (highbyte 0xFE)
#ifdef XK_dead_circumflex
  x11_oddmap[XK_dead_circumflex & 0xFF] = SK_CARET;
#endif
#ifdef XK_ISO_Level3_Shift
  x11_oddmap[XK_ISO_Level3_Shift & 0xFF] = SK_ALTGR;
#endif
  
  // Misc keymap (highbyte 0xFF)
  x11_miscmap[XK_BackSpace & 0xFF]    = SK_BACKSPACE;
  x11_miscmap[XK_Tab & 0xFF]          = SK_TAB;
  x11_miscmap[XK_Clear & 0xFF]        = SK_CLEAR;
  x11_miscmap[XK_Return & 0xFF]       = SK_RETURN;
  x11_miscmap[XK_Pause & 0xFF]        = SK_PAUSE;
  x11_miscmap[XK_Escape & 0xFF]       = SK_ESCAPE;
  x11_miscmap[XK_Delete & 0xFF]       = SK_DELETE;
  
  x11_miscmap[XK_KP_0 & 0xFF]         = SK_N_0;
  x11_miscmap[XK_KP_1 & 0xFF]         = SK_N_1;
  x11_miscmap[XK_KP_2 & 0xFF]         = SK_N_2;
  x11_miscmap[XK_KP_3 & 0xFF]         = SK_N_3;
  x11_miscmap[XK_KP_4 & 0xFF]         = SK_N_4;
  x11_miscmap[XK_KP_5 & 0xFF]         = SK_N_5;
  x11_miscmap[XK_KP_6 & 0xFF]         = SK_N_6;
  x11_miscmap[XK_KP_7 & 0xFF]         = SK_N_7;
  x11_miscmap[XK_KP_8 & 0xFF]         = SK_N_8;
  x11_miscmap[XK_KP_9 & 0xFF]         = SK_N_9;
  x11_miscmap[XK_KP_Insert & 0xFF]    = SK_N_0;
  x11_miscmap[XK_KP_End & 0xFF]       = SK_N_1;	
  x11_miscmap[XK_KP_Down & 0xFF]      = SK_N_2;
  x11_miscmap[XK_KP_Page_Down & 0xFF] = SK_N_3;
  x11_miscmap[XK_KP_Left & 0xFF]      = SK_N_4;
  x11_miscmap[XK_KP_Begin & 0xFF]     = SK_N_5;
  x11_miscmap[XK_KP_Right & 0xFF]     = SK_N_6;
  x11_miscmap[XK_KP_Home & 0xFF]      = SK_N_7;
  x11_miscmap[XK_KP_Up & 0xFF]        = SK_N_8;
  x11_miscmap[XK_KP_Page_Up & 0xFF]   = SK_N_9;
  x11_miscmap[XK_KP_Delete & 0xFF]    = SK_N_PERIOD;
  x11_miscmap[XK_KP_Decimal & 0xFF]   = SK_N_PERIOD;
  x11_miscmap[XK_KP_Divide & 0xFF]    = SK_N_DIVIDE;
  x11_miscmap[XK_KP_Multiply & 0xFF]  = SK_N_MULTIPLY;
  x11_miscmap[XK_KP_Subtract & 0xFF]  = SK_N_MINUS;
  x11_miscmap[XK_KP_Add & 0xFF]       = SK_N_PLUS;
  x11_miscmap[XK_KP_Enter & 0xFF]     = SK_N_ENTER;
  x11_miscmap[XK_KP_Equal & 0xFF]     = SK_N_EQUALS;
  
  x11_miscmap[XK_Up & 0xFF]           = SK_UP;
  x11_miscmap[XK_Down & 0xFF]         = SK_DOWN;
  x11_miscmap[XK_Right & 0xFF]        = SK_RIGHT;
  x11_miscmap[XK_Left & 0xFF]         = SK_LEFT;
  x11_miscmap[XK_Insert & 0xFF]       = SK_INSERT;
  x11_miscmap[XK_Home & 0xFF]         = SK_HOME;
  x11_miscmap[XK_End & 0xFF]          = SK_END;
  x11_miscmap[XK_Page_Up & 0xFF]      = SK_PAGEUP;
  x11_miscmap[XK_Page_Down & 0xFF]    = SK_PAGEDOWN;
  
  x11_miscmap[XK_F1 & 0xFF]           = SK_F1;
  x11_miscmap[XK_F2 & 0xFF]           = SK_F2;
  x11_miscmap[XK_F3 & 0xFF]           = SK_F3;
  x11_miscmap[XK_F4 & 0xFF]           = SK_F4;
  x11_miscmap[XK_F5 & 0xFF]           = SK_F5;
  x11_miscmap[XK_F6 & 0xFF]           = SK_F6;
  x11_miscmap[XK_F7 & 0xFF]           = SK_F7;
  x11_miscmap[XK_F8 & 0xFF]           = SK_F8;
  x11_miscmap[XK_F9 & 0xFF]           = SK_F9;
  x11_miscmap[XK_F10 & 0xFF]          = SK_F10;
  x11_miscmap[XK_F11 & 0xFF]          = SK_F11;
  x11_miscmap[XK_F12 & 0xFF]          = SK_F12;
  x11_miscmap[XK_F13 & 0xFF]          = SK_F13;
  x11_miscmap[XK_F14 & 0xFF]          = SK_F14;
  x11_miscmap[XK_F15 & 0xFF]          = SK_F15;
  
  x11_miscmap[XK_Num_Lock & 0xFF]     = SK_NUMLOCK;
  x11_miscmap[XK_Caps_Lock & 0xFF]    = SK_CAPSLOCK;
  x11_miscmap[XK_Scroll_Lock & 0xFF]  = SK_SCROLLOCK;
  x11_miscmap[XK_Shift_R & 0xFF]      = SK_RSHIFT;
  x11_miscmap[XK_Shift_L & 0xFF]      = SK_LSHIFT;
  x11_miscmap[XK_Control_R & 0xFF]    = SK_RCTRL;
  x11_miscmap[XK_Control_L & 0xFF]    = SK_LCTRL;
  x11_miscmap[XK_Alt_R & 0xFF]        = SK_RALT;
  x11_miscmap[XK_Alt_L & 0xFF]        = SK_LALT;
  x11_miscmap[XK_Meta_R & 0xFF]       = SK_RMETA;
  x11_miscmap[XK_Meta_L & 0xFF]       = SK_LMETA;
  x11_miscmap[XK_Super_L & 0xFF]      = SK_LWINDOWS;
  x11_miscmap[XK_Super_R & 0xFF]      = SK_RWINDOWS;
  x11_miscmap[XK_Mode_switch & 0xFF]  = SK_ALTGR;
  x11_miscmap[XK_Multi_key & 0xFF]    = SK_COMPOSE;
  
  x11_miscmap[XK_Help & 0xFF]         = SK_HELP;
  x11_miscmap[XK_Print & 0xFF]        = SK_PRINT;
  x11_miscmap[XK_Sys_Req & 0xFF]      = SK_SYSREQ;
  x11_miscmap[XK_Break & 0xFF]        = SK_BREAK;
  x11_miscmap[XK_Menu & 0xFF]         = SK_MENU;
  x11_miscmap[XK_Hyper_R & 0xFF]      = SK_MENU;
}

/* ******************************************************************** */

// Perform a full keyboard state update
void x11_keystate(sinp_device *dev, Display *d, char *keyvec) {
  char keyret[32];
  KeyCode xcode[SK_LAST];
  uint mod;
  Window w;
  sint i;
  sint j;
  uint mask;
  uchar newstate[SK_LAST];
  uchar *curstate;
  
  // Fetch pressed keys from X if not supplied
  if(!keyvec) {
    XQueryKeymap(d, keyret);
    keyvec = keyret;
  }

  // Query modifiers
  mod = SM_NONE;
  if(XQueryPointer(d, DefaultRootWindow(d), &w, &w,
		   &i, &j, &i, &j, &mask)) {
    x11_private *priv;
    priv = (x11_private*)dev->private;

    // Capslock
    if(mask & LockMask) {
      mod |= SM_CAPSLOCK;
    }
    if(mask & priv->mask_altgr) {
      mod |= SM_ALTGR;
    }
    if(mask & priv->mask_num) {
      mod |= SM_NUMLOCK;
    }
  }

  // Prepare new and current keystates
  memset(newstate, 0, sizeof(newstate));
  curstate = sinp_key_keystate(NULL);

  // Check each bit in the 32 bytes of the X keystate
  for(i=0; i<32; i++) {
    // No bits set
    if(!keyvec[i]) {
      continue;
    }

    // Scan bits
    for(j=0; j<8; j++) {
      if(keyvec[i] & (1<<j)) {
	// Find out what that key is
	sinp_keysym sks;
	KeyCode kc;
	
	kc = i << 3 | j;
	x11_translate(d, NULL, kc, &sks);
	
	newstate[sks.sym] = TRUE;
	xcode[sks.sym] = kc;
      }
    }
  }

  // Set state and fetch modifiers
  for(i=SK_FIRST+1; i<SK_LAST; i++) {  
    if(newstate[i]) {

      // Store new state of key
      curstate[i] = newstate[i];

      // Fetch normal modifiers
      switch(i) {
      case SK_LSHIFT:
	mod |= SM_LSHIFT;
	break;

      case SK_RSHIFT:
	mod |= SM_RSHIFT;
	break;

      case SK_LCTRL:
	mod |= SM_LCTRL;
	break;

      case SK_RCTRL:
	mod |= SM_RCTRL;
	break;

      case SK_LALT:
	mod |= SM_LALT;
	break;

      case SK_RALT:
	mod |= SM_RALT;
	break;

      case SK_LMETA:	
	mod |= SM_LMETA;
	break;

      case SK_RMETA:
	mod |= SM_RMETA;
	break;

      default:
	break;
      }
    }
  }

  // Correct for locking modifiers
  if(mod & SM_CAPSLOCK) {
    curstate[SK_CAPSLOCK] = TRUE;
  }
  else {
    curstate[SK_CAPSLOCK] = FALSE;
  }
  
  if(mod & SM_NUMLOCK) {
    curstate[SK_NUMLOCK] = TRUE;
  }
  else {
    curstate[SK_NUMLOCK] = FALSE;
  }

  keyboard_setmodifier(mod);
}

/* ******************************************************************** */

// Translate X11 keysym to sinp keysym
inline sinp_keysym *x11_translate(Display *d, XKeyEvent *xkey,
				  KeyCode kc, sinp_keysym *keysym) {
  KeySym xsym;

  // Basic sinp keysym
  keysym->scancode = kc;
  keysym->sym = SK_UNKNOWN;
  keysym->mod = SM_NONE;

  // Handle X keysym
  xsym = XKeycodeToKeysym(d, kc, 0);
  if(xsym) {
    // Keymap information is in the high byte
    switch(xsym >> 8) {
      
    case 0x00: // Latin 1
    case 0x01: // Latin 2
    case 0x02: // Latin 3
    case 0x03: // Latin 4
    case 0x04: // Katakana
    case 0x05: // Arabic
    case 0x06: // Cyrillic
    case 0x07: // Greek
    case 0x08: // Technical
    case 0x0A: // Publishing
    case 0x0C: // Hebrew
    case 0x0D: // Thai
      // Normal ASCII keymap
      keysym->sym = (sinp_key)(xsym & 0xFF);
      
      // Fix lowercase
      if((keysym->sym >= 'A') && (keysym->sym <= 'Z')) {
	keysym->sym += 'a'-'A';
      }
      break;
      
    case 0xFE: // Odd
      keysym->sym = x11_oddmap[xsym & 0xFF];
      break;

    case 0xFF: // Misc
      keysym->sym = x11_miscmap[xsym & 0xFF];
      break;
      
    default: // Unhandled
      debug("x11_translate: unhandled map 0x%04x", (uint)xsym);
      break;
    }
  }

  // Handle special scancodes
  else {
    switch(kc) {
    case 115:
      keysym->sym = SK_LWINDOWS;
      break;

    case 116:
      keysym->sym = SK_RWINDOWS;
      break;

    case 117:
      keysym->sym = SK_MENU;
      break;
    }
  }

  // Done
  return keysym;
}

/* ******************************************************************** */

// Get masks of modifiers
void x11_modmasks(Display *d, sinp_device *dev) {
  XModifierKeymap *xmods;
  int i;
  int j;
  uint n;
  uint mask;
  x11_private *priv;

  priv = (x11_private*)dev->private;

  // Query
  xmods = XGetModifierMapping(d);
  n = xmods->max_keypermod;

  // Decode (lifted from SDL, thanks!)
  for(i = 3; i < 8; i++) {
    for(j = 0; j < n; j++) {

      KeyCode kc = xmods->modifiermap[i * n + j];
      KeySym ks = XKeycodeToKeysym(d, kc, 0);

      mask = 1 << i;

      switch(ks) {
      case XK_Num_Lock:
	priv->mask_num = mask;
	break;

      case XK_Alt_L:
	priv->mask_lalt = mask;
	break;

      case XK_Alt_R:
	priv->mask_ralt = mask;
	break;

      case XK_Meta_L:
	priv->mask_lmeta = mask;
	break;

      case XK_Meta_R:
	priv->mask_rmeta = mask;
	break;

      case XK_Mode_switch:
	priv->mask_altgr = mask;
	break;
      }
    }
  }
  XFreeModifiermap(xmods);
}

/* ******************************************************************** */
