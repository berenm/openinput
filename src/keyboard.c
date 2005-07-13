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
#include <string.h>
#include "sinp.h"
#include "internal.h"

// Globals
static sinp_device *keydev;
static uchar keystate[SK_LAST];
static char *keynames[SK_LAST];
static uint modstate;

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
  memset(keystate, FALSE, sizeof(keystate)/sizeof(keystate[0]));
  memset(keynames, 0, sizeof(keynames)/sizeof(keynames[0]));
  modstate = 0;

  // Find default/first mouse device
  i = 0;
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

  // Disable key-repeat
  sinp_key_repeat(0, 0);

  // Fill keyboard names
  keyboard_fillnames(keynames);

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
      // Locks only send 'down' events, so bail out
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

  name = NULL;
  if(key < SK_LAST) {
    name = keynames[key];
  }

  return name;
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
