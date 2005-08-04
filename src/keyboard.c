/*
 * keyboard.c : Keyboard state interface
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

// Includes
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openinput.h"
#include "internal.h"

// Globals
static oi_device *keydev;
static uchar keystate[OIK_LAST];
static uint modstate;
static char *keynames[OIK_LAST];

// Key repeat
struct {
  uchar first;
  sint delay;
  sint interval;
  uint timestamp;
  oi_event ev;
} keyboard_keyrep;

/* ******************************************************************** */

// Initialize keyboard state manager (internal)
sint keyboard_init() {
  int i;

  // Clear tables
  memset(keystate, FALSE, TABLESIZE(keystate));
  memset(keynames, 0, TABLESIZE(keynames));
  modstate = OIM_NONE;

  // Disable key-repeat
  oi_key_repeat(0, 0);

  // Fill keyboard names
  keyboard_fillnames(keynames);

  // Find default/first mouse device
  i = 1;
  while((keydev = device_get(i)) != NULL) {
    if((keydev->provides & OI_PRO_KEYBOARD) == OI_PRO_KEYBOARD) {
      break;
    }
    i++;
  }

  // We really, really want a keyboard!
  if(keydev == NULL) {
    return OI_ERR_NO_DEVICE;
  }

  debug("keyboard_init: keyboard device is '%s'", keydev->name);

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Update keyboard/modifier state (internal)
void keyboard_update(oi_keysym *keysym, sint state, uchar postdev) {
  oi_type type;
  oi_event ev;
  uint newmod;
  uchar repeat;
  
  // Temporary new modifier state
  newmod = modstate;
  repeat = FALSE;
  
  // Check for modifier updates
  if(state) {
    // Add modifiers to the event
    keysym->mod = newmod;

    // Handle each modifier-key
    switch(keysym->sym) {
      
    case OIK_NUMLOCK:
      // Locks are special
      newmod ^= OIM_NUMLOCK;
      if(!(newmod & OIM_NUMLOCK)) {
	state = FALSE;
      }
      keysym->mod = newmod;
      break;
      
    case OIK_CAPSLOCK:
      // Locks are special
      newmod ^= OIM_CAPSLOCK;
      if(!(newmod & OIM_CAPSLOCK)) {
	state = FALSE;
      }
      keysym->mod = newmod;
      break;

    case OIK_SCROLLOCK:
      // Locks are special
      newmod ^= OIM_SCROLLOCK;
      if(!(newmod & OIM_SCROLLOCK)) {
	state = FALSE;
      }
      keysym->mod = newmod;
      break;

    case OIK_RSHIFT:
      newmod |= OIM_RSHIFT;
      break;

    case OIK_LSHIFT:
      newmod |= OIM_LSHIFT;
      break;

    case OIK_RCTRL:
      newmod |= OIM_RCTRL;
      break;

    case OIK_LCTRL:
      newmod |= OIM_LCTRL;
      break;

    case OIK_RALT:
      newmod |= OIM_RALT;
      break;

    case OIK_LALT:
      newmod |= OIM_LALT;
      break;

    case OIK_RMETA:
      newmod |= OIM_RMETA;
      break;

    case OIK_LMETA:
      newmod |= OIM_LMETA;
      break;

    case OIK_ALTGR:
      newmod |= OIM_ALTGR;
      break;

    default:
      // Not a modifier and key down - we can do repeats ;)
      repeat = TRUE;
      break;
    }
  }
  else {
    // Handle each modifier-key
    switch(keysym->sym) {
      
    case OIK_NUMLOCK:
    case OIK_CAPSLOCK:
    case OIK_SCROLLOCK:
      // Only send down-events on num and capslock
      return;

    case OIK_RSHIFT:
      newmod &= ~OIM_RSHIFT;
      break;

    case OIK_LSHIFT:
      newmod &= ~OIM_LSHIFT;
      break;

    case OIK_RCTRL:
      newmod &= ~OIM_RCTRL;
      break;

    case OIK_LCTRL:
      newmod &= ~OIM_LCTRL;
      break;

    case OIK_RALT:
      newmod &= ~OIM_RALT;
      break;

    case OIK_LALT:
      newmod &= ~OIM_LALT;
      break;

    case OIK_RMETA:
      newmod &= ~OIM_RMETA;
      break;

    case OIK_LMETA:
      newmod &= ~OIM_LMETA;
      break;

    case OIK_ALTGR:
      newmod &= ~OIM_ALTGR;
      break;

    default:
      break;
    }     

    // We're removing modifiers, store event mod
    keysym->mod = newmod;
  }

  // Handle down and up type event
  if(state) {
    type = OI_KEYDOWN;
  }
  else {
    type = OI_KEYUP;

    // Disable repeat if key matches
    if(keyboard_keyrep.timestamp &&
       (keyboard_keyrep.ev.key.keysym.sym == keysym->sym)) {
      keyboard_keyrep.timestamp = 0;
    }
  }

  // If key state didn't change, bail out
  if(keystate[keysym->sym] == state) {
    return;
  }
  
  // Store new states
  keystate[keysym->sym] = state;
  modstate = newmod;

  // Setup event
  ev.type = type;
  ev.key.device = postdev;
  ev.key.state = state;
  ev.key.keysym = *keysym;

  // Update key-repeat if enabled and repeatable key
  if(repeat && (keyboard_keyrep.delay > 0)) {
    keyboard_keyrep.ev = ev;
    keyboard_keyrep.first = TRUE;
    keyboard_keyrep.timestamp = oi_getticks();
  }

  // Postal services
  if(postdev) {
    queue_add(&ev);
  }
}

/* ******************************************************************** */

// Repeat keyboard events (internal)
void keyboard_dorepeat() {
  uint now;
  sint interval;

  // Available?
  if(keyboard_keyrep.timestamp) {
    now = oi_getticks();
    interval = now - keyboard_keyrep.timestamp;

    // New keypress?
    if(keyboard_keyrep.first) {

      // Wait for delay to expire
      if(interval > keyboard_keyrep.delay) {
	keyboard_keyrep.first = FALSE;
	keyboard_keyrep.timestamp = now;
      }
    }

    // Really, do a repeat by sending an event
    else if(interval > keyboard_keyrep.interval) {
      keyboard_keyrep.timestamp = now;
      queue_add(&keyboard_keyrep.ev);
    }
  }
}

/* ******************************************************************** */

// Set modifier mask
void keyboard_setmodifier(uint newmod) {
  modstate = newmod;
}

/* ******************************************************************** */

// Return current modifier state (public)
uint oi_key_modstate() {
  return modstate;
}

/* ******************************************************************** */

// Return key state table or size (public)
uchar *oi_key_keystate(sint *num) {
  if(num != NULL) {
    *num = OIK_LAST;
  }
  return keystate;
}

/* ******************************************************************** */

// Return name for key (public)
char *oi_key_getname(oi_key key) {
  char *name;

  name = keynames[OIK_UNKNOWN];

  if(key < OIK_LAST) {
    name = keynames[key];
  }

  return name;
}

/* ******************************************************************** */

// Byte-for-byte keycode-keyname check (internal)
inline oi_key keyboard_scangetkey(char *name, oi_key first, oi_key last) {
  oi_key k;

  for(k=first; k<=last; k++) {
    if(strcmp(name, oi_key_getname(k)) == 0) {
      return k;
    }
  }
  return OIK_UNKNOWN;
}

/* ******************************************************************** */

// Return key for name (public)
oi_key oi_key_getcode(char *name) {
  int i;
  oi_key k;

  // Dummies
  if(!name) {
    return OIK_UNKNOWN;
  }
  if((strlen(name) < OI_MIN_KEYLENGTH) ||
     (strlen(name) > OI_MAX_KEYLENGTH)) {
    return OIK_UNKNOWN;
  }

  // Check prefix
  if(strncmp(name, "key_", 4) != 0) {
    return OIK_UNKNOWN;
  }

  // Catch this one now
  if(strcmp(name, "key_unknown") == 0) {
    return OIK_UNKNOWN;
  }

  // Just to be sure...
  k = OIK_UNKNOWN;

  // Letter or digit
  if(strlen(name) == 5) {
    if((name[4] >= 'a') && (name[4] <= 'z')) {
      return OIK_A + (name[4]-'a');
    }
    if((name[4] >= '0') && (name[4] <= '9')) {
      return OIK_0 + (name[4]-'0');
    }

    return OIK_UNKNOWN;
  }

  // Function keys (only these and 'f' starts with 'f')
  if(name[4] == 'f') {
    i = atoi(name+5);
    if((i>=1) && (i<=15)) {
      return OIK_F1 + i - 1;
    }
    
    return OIK_UNKNOWN;
  }

  // International
  if(strncmp(name, "key_int", 7) == 0) {
    i = atoi(name+7);
    if((i>=0) && (i<=95)) {
      return OIK_INT_0 + i;
    }
    
    return OIK_UNKNOWN;
  }

  // Numeric keypad non-numbers (num_period -> num_equals)
  if((k = keyboard_scangetkey(name, OIK_N_PERIOD, OIK_N_EQUALS)) != OIK_UNKNOWN) {
    return k;
  }

  // Numeric keypad numbers
  if(strncmp(name, "key_num_", 8) == 0) {
    i = atoi(name+8);
    if((i>=0) && (i<=9)) {
      return OIK_N_0 + i;
    }
  }

  // Backspace -> Slash
  if((k = keyboard_scangetkey(name, OIK_BACKSPACE, OIK_SLASH)) != OIK_UNKNOWN) {
    return k;
  }

  // Colon -> Backquote
  if((k = keyboard_scangetkey(name, OIK_COLON, OIK_BACKQUOTE)) != OIK_UNKNOWN) {
    return k;
  }

  // Delete
  if(strcmp(name, "key_delete") == 0) {
    return OIK_DELETE;
  }

  // Up -> pagedown
  if((k = keyboard_scangetkey(name, OIK_UP, OIK_PAGEDOWN)) != OIK_UNKNOWN) {
    return k;
  }

  // Numlock -> undo
  if((k = keyboard_scangetkey(name, OIK_NUMLOCK, OIK_UNDO)) != OIK_UNKNOWN) {
    return k;
  }

  // No more keys left
  return OIK_UNKNOWN;
}

/* ******************************************************************** */

// Setup key repeat system (public)
sint oi_key_repeat(sint delay, sint interval) {
  // Dummy check
  if((delay < 0) || (interval < 0)) {
    return OI_ERR_PARAM;
  }

  // Setup
  keyboard_keyrep.first = FALSE;
  keyboard_keyrep.delay = delay;
  keyboard_keyrep.interval = interval;
  keyboard_keyrep.timestamp = 0;
  
  return OI_ERR_OK;
}

/* ******************************************************************** */
