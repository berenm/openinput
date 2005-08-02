/*
 * keyboard.c : Keyboard state interface
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sinp.h"
#include "internal.h"

// Globals
static sinp_device *keydev;
static uchar keystate[SK_LAST];
static uint modstate;
static char *keynames[SK_LAST];
static uchar keyaction[SK_LAST];

// Key repeat
struct {
  uchar first;
  sint delay;
  sint interval;
  uint timestamp;
  sinp_event ev;
} keyboard_keyrep;

/* ******************************************************************** */

// Initialize keyboard state manager (internal)
sint keyboard_init() {
  int i;

  // Clear tables
  memset(keystate, FALSE, TABLESIZE(keystate));
  memset(keyaction, FALSE, TABLESIZE(keyaction));
  memset(keynames, 0, TABLESIZE(keynames));
  modstate = SM_NONE;

  // Disable key-repeat
  sinp_key_repeat(0, 0);

  // Fill keyboard names
  keyboard_fillnames(keynames);

  // Find default/first mouse device
  i = 1;
  while((keydev = device_get(i)) != NULL) {
    if((keydev->provides & SINP_PRO_KEYBOARD) == SINP_PRO_KEYBOARD) {
      break;
    }
    i++;
  }

  // We really, really want a keyboard!
  if(keydev == NULL) {
    return SINP_ERR_NO_DEVICE;
  }

  debug("keyboard_init: keyboard device is '%s'", keydev->name);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Update keyboard/modifier state (internal)
void keyboard_update(sinp_keysym *keysym, sint state, uchar postdev) {
  sinp_type type;
  sinp_event ev;
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
      
    case SK_NUMLOCK:
      // Locks are special
      newmod ^= SM_NUMLOCK;
      if(!(newmod & SM_NUMLOCK)) {
	state = FALSE;
      }
      keysym->mod = newmod;
      break;
      
    case SK_CAPSLOCK:
      // Locks are special
      newmod ^= SM_CAPSLOCK;
      if(!(newmod & SM_CAPSLOCK)) {
	state = FALSE;
      }
      keysym->mod = newmod;
      break;

    case SK_SCROLLOCK:
      // Locks are special
      newmod ^= SM_SCROLLOCK;
      if(!(newmod & SM_SCROLLOCK)) {
	state = FALSE;
      }
      keysym->mod = newmod;
      break;

    case SK_RSHIFT:
      newmod |= SM_RSHIFT;
      break;

    case SK_LSHIFT:
      newmod |= SM_LSHIFT;
      break;

    case SK_RCTRL:
      newmod |= SM_RCTRL;
      break;

    case SK_LCTRL:
      newmod |= SM_LCTRL;
      break;

    case SK_RALT:
      newmod |= SM_RALT;
      break;

    case SK_LALT:
      newmod |= SM_LALT;
      break;

    case SK_RMETA:
      newmod |= SM_RMETA;
      break;

    case SK_LMETA:
      newmod |= SM_LMETA;
      break;

    case SK_ALTGR:
      newmod |= SM_ALTGR;
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
      
    case SK_NUMLOCK:
    case SK_CAPSLOCK:
    case SK_SCROLLOCK:
      // Only send down-events on num and capslock
      return;

    case SK_RSHIFT:
      newmod &= ~SM_RSHIFT;
      break;

    case SK_LSHIFT:
      newmod &= ~SM_LSHIFT;
      break;

    case SK_RCTRL:
      newmod &= ~SM_RCTRL;
      break;

    case SK_LCTRL:
      newmod &= ~SM_LCTRL;
      break;

    case SK_RALT:
      newmod &= ~SM_RALT;
      break;

    case SK_LALT:
      newmod &= ~SM_LALT;
      break;

    case SK_RMETA:
      newmod &= ~SM_RMETA;
      break;

    case SK_LMETA:
      newmod &= ~SM_LMETA;
      break;

    case SK_ALTGR:
      newmod &= ~SM_ALTGR;
      break;

    default:
      break;
    }     

    // We're removing modifiers, store event mod
    keysym->mod = newmod;
  }

  // Handle down and up type event
  if(state) {
    type = SINP_KEYDOWN;
  }
  else {
    type = SINP_KEYUP;

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
    keyboard_keyrep.timestamp = sinp_getticks();
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
    now = sinp_getticks();
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
uint sinp_key_modstate() {
  return modstate;
}

/* ******************************************************************** */

// Return key state table or size (public)
uchar *sinp_key_keystate(sint *num) {
  if(num != NULL) {
    *num = SK_LAST;
  }
  return keystate;
}

/* ******************************************************************** */

// Return name for key (public)
char *sinp_key_getname(sinp_key key) {
  char *name;

  name = keynames[SK_UNKNOWN];

  if(key < SK_LAST) {
    name = keynames[key];
  }

  return name;
}

/* ******************************************************************** */

// Byte-for-byte keycode-keyname check (internal)
inline sinp_key keyboard_scangetkey(char *name, sinp_key first, sinp_key last) {
  sinp_key k;

  for(k=first; k<=last; k++) {
    if(strcmp(name, sinp_key_getname(k)) == 0) {
      return k;
    }
  }
  return SK_UNKNOWN;
}

/* ******************************************************************** */

// Return key for name (public)
sinp_key sinp_key_getcode(char *name) {
  int i;
  sinp_key k;

  // Dummies
  if(!name) {
    return SK_UNKNOWN;
  }
  if((strlen(name) < SINP_MIN_KEYLENGTH) ||
     (strlen(name) > SINP_MAX_KEYLENGTH)) {
    return SK_UNKNOWN;
  }

  // Check prefix
  if(strncmp(name, "key_", 4) != 0) {
    return SK_UNKNOWN;
  }

  // Catch this one now
  if(strcmp(name, "key_unknown") == 0) {
    return SK_UNKNOWN;
  }

  // Just to be sure...
  k = SK_UNKNOWN;

  // Letter or digit
  if(strlen(name) == 5) {
    if((name[4] >= 'a') && (name[4] <= 'z')) {
      return SK_A + (name[4]-'a');
    }
    if((name[4] >= '0') && (name[4] <= '9')) {
      return SK_0 + (name[4]-'0');
    }

    return SK_UNKNOWN;
  }

  // Function keys (only these and 'f' starts with 'f')
  if(name[4] == 'f') {
    i = atoi(name+5);
    if((i>=1) && (i<=15)) {
      return SK_F1 + i - 1;
    }
    
    return SK_UNKNOWN;
  }

  // International
  if(strncmp(name, "key_int", 7) == 0) {
    i = atoi(name+7);
    if((i>=0) && (i<=95)) {
      return SK_INT_0 + i;
    }
    
    return SK_UNKNOWN;
  }

  // Numeric keypad non-numbers (num_period -> num_equals)
  if((k = keyboard_scangetkey(name, SK_N_PERIOD, SK_N_EQUALS)) != SK_UNKNOWN) {
    return k;
  }

  // Numeric keypad numbers
  if(strncmp(name, "key_num_", 8) == 0) {
    i = atoi(name+8);
    if((i>=0) && (i<=9)) {
      return SK_N_0 + i;
    }
  }

  // Backspace -> Slash
  if((k = keyboard_scangetkey(name, SK_BACKSPACE, SK_SLASH)) != SK_UNKNOWN) {
    return k;
  }

  // Colon -> Backquote
  if((k = keyboard_scangetkey(name, SK_COLON, SK_BACKQUOTE)) != SK_UNKNOWN) {
    return k;
  }

  // Delete
  if(strcmp(name, "key_delete") == 0) {
    return SK_DELETE;
  }

  // Up -> pagedown
  if((k = keyboard_scangetkey(name, SK_UP, SK_PAGEDOWN)) != SK_UNKNOWN) {
    return k;
  }

  // Numlock -> undo
  if((k = keyboard_scangetkey(name, SK_NUMLOCK, SK_UNDO)) != SK_UNKNOWN) {
    return k;
  }

  // No more keys left
  return SK_UNKNOWN;
}

/* ******************************************************************** */

// Setup key repeat system (public)
sint sinp_key_repeat(sint delay, sint interval) {
  // Dummy check
  if((delay < 0) || (interval < 0)) {
    return SINP_ERR_PARAM;
  }

  // Setup
  keyboard_keyrep.first = FALSE;
  keyboard_keyrep.delay = delay;
  keyboard_keyrep.interval = interval;
  keyboard_keyrep.timestamp = 0;
  
  return SINP_ERR_OK;
}

/* ******************************************************************** */
