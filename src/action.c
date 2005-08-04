/*
 * action.c : The action mapper functions
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

// Global state table and count
static uchar *action_state;
static sint action_count;

// Lookup table for keyboard
static uint action_keyboard[OIK_LAST];

// Lookup table for mouse
static uint action_mouse[OIP_LAST];

/* ******************************************************************** */

// Initialize the action mapper (internal)
sint action_init() {
  debug("action_init");

  // Map is not initialized
  action_state = NULL;
  action_count = 0;

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Install user defined actionmap (public)
sint oi_action_install(oi_actionmap *map, sint num) {
  sint i;
  sint j;
  sint big;

  debug("oi_action_install");

  // Dummy
  if((map == NULL) || (num <= 0)) {
    return OI_ERR_PARAM;
  }

  // Scan for uniqueness of actionid (asume unsorted set)
  big = 0;
  for(i=0; i<num; i++) {
    for(j=i+1; j<num; j++) {
      if(map[i].actionid == map[j].actionid) {
	return OI_ERR_NOT_UNIQUE;
      }
    }

    // We also need the biggest actionid
    if(map[i].actionid > big) {
      big = map[i].actionid;
    }
  }

  debug("oi_action_install: map is unique, biggest id: %i", big);

  // Validate elements
  for(i=0; i<num; i++) {
    if(oi_action_validate(&(map[i])) != OI_ERR_OK) {
      return OI_ERR_PARAM;
    }
  }

  debug("oi_action_install: map is valid");

  // Clean lookup tables
  memset(action_keyboard, 0, TABLESIZE(action_keyboard));
  memset(action_mouse, 0, TABLESIZE(action_mouse));

  // Free old state table
  if(action_count > 0) {
    free(action_state);
  }

  // Alloc state table and clear it
  action_count = big;
  action_state = (uchar*)malloc(big);
  memset(action_state, 0, big);

  // Parse event and fill the lookup tables
  for(i=0; i<num; i++) {
    // Action is keyboard
    if((j = oi_key_getcode(map[i].name))) {
      action_keyboard[j] = map[i].actionid;

      debug("oi_action_install: keyboard action:\t id:%u name:'%s'",
	    map[i].actionid, map[i].name);
    }

    // Action is mouse
    else if((j = oi_mouse_getcode(map[i].name))) {
      action_mouse[j] = map[i].actionid;

      debug("oi_action_install: mouse action:\t id:%u name:'%s'",
	    map[i].actionid, map[i].name);
    }

    // Add more lookup tables here
  }

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Validate single actionmap structure (public)
sint oi_action_validate(oi_actionmap *map) {
  // Check the basic of the structure
  if(map == NULL) {
    return OI_ERR_PARAM;
  }
  if(map->actionid == 0) {
    return OI_ERR_PARAM;
  }
  if(map->name == NULL) {
    return OI_ERR_PARAM;
  }
  if((strlen(map->name) < OI_MIN_KEYLENGTH) ||
     (strlen(map->name) > OI_MAX_KEYLENGTH)) {
    return OI_ERR_PARAM;
  }

  // Does event name exist? Simply ask all managers
  if((oi_key_getcode(map->name) == OIK_UNKNOWN) &&
     (oi_mouse_getcode(map->name) == OIP_UNKNOWN)) {
    return OI_ERR_NO_NAME;
  }
  
  // Everything should be ok now
  return OI_ERR_OK;
}

/* ******************************************************************** */

// Return state table (public)
uchar *oi_action_actionstate(sint *num) {
  if(num != NULL) {
    *num = action_count;
  }
  return action_state;
}

/* ******************************************************************** */

// Check map and change state/generate event (internal)
inline void action_process(oi_event *evt) {
  static oi_event act;
  int i;
  int state = 0;

  // Dummy check
  if((action_state == NULL) || (action_count <= 0)) {
    return;
  }

  // Set defaults
  act.type = OI_ACTION;
  act.action.device = 0;
  act.action.data1 = 0;
  act.action.data2 = 0;
  act.action.data3 = 0;

  /* We handle the discrete events first (ie. buttons) to allow
   * for the simple "if {} else if {}" optimization.
   * After that, the 'real' events are handled, as these require
   * zero'ing the state table even if event does not match
   */

  // Discrete: Keyboard
  if((evt->type == OI_KEYUP) ||
     (evt->type == OI_KEYDOWN)) {

    i = evt->key.keysym.sym;
    
    // Check trigger
    if(action_keyboard[i] != 0) {
      act.action.device = evt->key.device;
      act.action.actionid = action_keyboard[i];
      act.action.state = (evt->type == OI_KEYDOWN);

      debug("action_process: %u (keyboard)", act.action.actionid);
    }
  }

  // Discrete: Mouse button
  else if((evt->type == OI_MOUSEBUTTONUP) ||
	  (evt->type == OI_MOUSEBUTTONDOWN)) {

    i = evt->button.button;

    // Mouse wheel is discrete
    if(((i == OIP_WHEEL_UP) && (action_mouse[i] != 0)) ||
       ((i == OIP_WHEEL_DOWN) && (action_mouse[i] != 0))) {

      // Only trigger on the down-event
      if(evt->type == OI_MOUSEBUTTONDOWN) {

	act.action.device = evt->button.device;
	act.action.actionid = action_mouse[i];
	act.action.state = TRUE;
	
	debug("action_process: %u (mouse wheel)", act.action.actionid);
      }
    }

    // Standard trigger check
    else if(action_mouse[i] != 0) {
      
      act.action.device = evt->button.device;
      act.action.actionid = action_mouse[i];
      act.action.state = (evt->type == OI_MOUSEBUTTONDOWN);
      
      debug("action_process: %u (mouse button)", act.action.actionid);
    }
  }

  // Real: Mouse move
  if((evt->type == OI_MOUSEMOVE) &&
     (action_mouse[OIP_MOTION] != 0)) {
    
    act.action.device = evt->move.device;
    act.action.actionid = action_mouse[OIP_MOTION];
    act.action.state = TRUE;
    act.action.data1 = evt->move.relx;
    act.action.data2 = evt->move.rely;

    debug("action_process: %u (mouse motion)", act.action.actionid);
  }
  else {
    // Make sure state table is zero'ed
    action_state[action_mouse[OIP_MOTION]] = FALSE;
  }

  // If nothing was changed, the device index is zero
  if(act.action.device == 0) {
    return;
  }

  // Update state table
  action_state[act.action.actionid] = state;

  // Post the action event
  queue_add(&act);
}

/* ******************************************************************** */
