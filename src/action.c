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
static oi_aclink *action_keyboard[OIK_LAST];

// Lookup table for mouse
static oi_aclink *action_mouse[OIP_LAST];

/* ******************************************************************** */

/**
 * @ingroup IAction
 * @brief Initialize action mapper
 *
 * Must be called on library initialization.
 */
sint action_init() {
  debug("action_init");

  // Map is not initialized
  action_state = NULL;
  action_count = 0;

  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup PAction
 * @brief Install new action map
 *
 * @param map pointer to the action map
 * @param num number of elements in map
 * @returns errorcode, see @ref PErrors
 *
 * Free current action map (if any) and install new one.
 */
sint oi_action_install(oi_actionmap *map, sint num) {
  sint i;
  sint j;
  sint big;
  oi_aclink *last;

  debug("oi_action_install");

  // Dummy
  if((map == NULL) || (num <= 0)) {
    return OI_ERR_PARAM;
  }

  // Get largest action id
  big = 0;
  for(i=0; i<num; i++) {
    if(map[i].actionid > big) {
      big = map[i].actionid;
    }
  }

  debug("oi_action_install: highest action id: %i", big);

  // Validate elements
  for(i=0; i<num; i++) {
    if(oi_action_validate(&(map[i])) != OI_ERR_OK) {
      return OI_ERR_PARAM;
    }
  }

  debug("oi_action_install: map is valid");

  // Clean lookup tables
  action_cleartable(action_keyboard, OIK_LAST);
  action_cleartable(action_mouse, OIP_LAST);

  // Free old state table
  if(action_count > 0) {
    free(action_state);
  }

  // Alloc state table and clear it
  action_count = big;
  action_state = (uchar*)malloc(big);
  memset(action_state, 0, big);

  /* Parse event and fill the lookup tables
   * The tables are arrays of linked lists. Actions can be triggered
   * by multiple devices, and can therefore belong to more tables
   *
   */
  for(i=0; i<num; i++) {

    // Action is keyboard
    if((j = oi_key_getcode(map[i].name))) {

      // Alloc new tail and fill it
      last = action_tail(&action_keyboard[j], TRUE);
      last->action = map[i].actionid;
      last->device = map[i].device;

      debug("oi_action_install: keyboard action:\t id:%u name:'%s'",
	    map[i].actionid, map[i].name);
    }

    // Action is mouse
    if((j = oi_mouse_getcode(map[i].name))) {
      
      // Alloc new tail and fill it
      last = action_tail(&action_mouse[j], TRUE);
      last->action = map[i].actionid;
      last->device = map[i].device;

      debug("oi_action_install: mouse action:\t id:%u name:'%s'",
	    map[i].actionid, map[i].name);
    }

    //FIXME Add more lookup tables here
  }

#ifdef DEBUG
 {
   debug("oi_action_install: begin action table printout");
   for(i=0; i<OIK_LAST; i++) {
     last = action_keyboard[i];
     while(last != NULL) {
       debug("keyboard \t id:%i \t dev:%i \t action:%i",
	     i, last->device, last->action);
       last = last->next;
     }
   }
   for(i=0; i<OIP_LAST; i++) {
     last = action_mouse[i];
     while(last != NULL) {
       debug("mouse \t id:%i \t dev:%i \t action:%i",
	     i, last->device, last->action);
       last = last->next;
     }
   }
   debug("oi_action_install: end action table printout");
 }
#endif

  return OI_ERR_OK;
}

/* ******************************************************************** */

// Validate single actionmap structure (public)
/**
 * @ingroup PAction
 * @brief Validate a single action map
 *
 * @param map pointer to action map
 * @returns errorcode, see @ref PErrors
 *
 * Check if action map is valid. For example,
 * check that the event name exists
 */
sint oi_action_validate(oi_actionmap *map) {
  uint u;
  int i;

  // Simple checks
  if(map == NULL) {
    return OI_ERR_PARAM;
  }
  if(map->actionid == 0) {
    return OI_ERR_PARAM;
  }
  if(map->name == NULL) {
    return OI_ERR_PARAM;
  }

  // Range checks
  if((map->device != 0) && (device_get(map->device) == NULL)) {
    return OI_ERR_NO_DEVICE;
  }
  if((strlen(map->name) < OI_MIN_KEYLENGTH) ||
     (strlen(map->name) > OI_MAX_KEYLENGTH)) {
    return OI_ERR_PARAM;
  }

  // Bound to a specific device, does name exist?
  if(map->device != 0) {
    // Get device provide flags
    u = device_get(map->device)->provides;
    i = FALSE;

    // Test provide flags
    if(u & OI_PRO_KEYBOARD) {
      i |= (oi_key_getcode(map->name) != OIK_UNKNOWN);
    }
    if(u & OI_PRO_MOUSE) {
      i |= (oi_mouse_getcode(map->name) != OIP_UNKNOWN);
    }
    //FIXME add more code-lookups here

    // If noone knew about the key, bail
    if(i == FALSE) {
      return OI_ERR_NO_NAME;
    }
  }

  // Generic device - ask all if key exists
  else if((oi_key_getcode(map->name) == OIK_UNKNOWN) &&
	  (oi_mouse_getcode(map->name) == OIP_UNKNOWN)) {
    return OI_ERR_NO_NAME;
  }
  
  // Everything should be ok now
  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup PAction
 * @brief Get pointer to action state table
 *
 * @param num pointer to integer to be filled with number of states.
 * Can be NULL
 * @returns pointer to state table
 *
 * Obtain pointer to action state table. The structure is internal
 * and must NOT be freed or altered!
 */
uchar *oi_action_actionstate(sint *num) {
  if(num != NULL) {
    *num = action_count;
  }
  return action_state;
}

/* ******************************************************************** */

/**
 * @ingroup IAction
 * @brief Process event and possible generate an action
 *
 * @param evt pointer to event
 *
 * Parse given event, and if action map exists, generate
 * the action event. The action event is automatically
 * injected into the queue.
 */
void action_process(oi_event *evt) {
  static oi_event act;
  oi_aclink *link;
  int i;

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

  /**
   * @note
   * We handle the discrete events first (ie. buttons) to allow
   * for the simple "if {} else if {}" optimization.
   * After that, the 'real' (axis) events are handled, as these require
   * zero'ing the state table even if event does not match
   *
   * The angle of attack when analysing an event is as follows
   * @li Check event type (evt->type)
   * @li Calculate table offset (keysym/button index)
   * @li Get table entry, which is a linked list (action_manager[index])
   * @li Parse linked list (while(item != NULL))
   * @li Check each list item for device index (zero or match the event poster)
   * @li Setup the action event structure and post it
   *
   * The above list assumes that all steps are successfull, ie. that
   * types and indexes match etc.
   */

  // Discrete: Keyboard
  if((evt->type == OI_KEYUP) ||
     (evt->type == OI_KEYDOWN)) {

    // Check trigger
    i = evt->key.keysym.sym;
    link = action_keyboard[i];
    while(link != NULL) {

      // Match device
      if((link->device == 0) || (link->device == evt->key.device)) {
	act.action.device = evt->key.device;
	act.action.actionid = link->action;
	act.action.state = (evt->type == OI_KEYDOWN);
	action_statepost(&act);
	debug("action_process: %u (keyboard)", act.action.actionid);
      }
      
      // Next item
      link = link->next;
    }
  }

  // Discrete: Mouse button
  else if((evt->type == OI_MOUSEBUTTONUP) ||
	  (evt->type == OI_MOUSEBUTTONDOWN)) {

    i = evt->button.button;
    link = action_mouse[i];
    while(link != NULL) {

      /* Special handling for mouse scroll wheels!
       * We only want a single event, and that's the button-down
       * Otherwise, do the standard match device etc.
       */
      if(((i == OIP_WHEEL_UP) || (i == OIP_WHEEL_DOWN)) && 
	 (evt->type == OI_MOUSEBUTTONDOWN) &&
	 ((link->device == 0) || (link->device == evt->button.device))) {
	act.action.device = evt->button.device;
	act.action.actionid = link->action;
	act.action.state = TRUE;
	action_statepost(&act);
	debug("action_process: %u (mouse wheel)", act.action.actionid);
      }    
      // Normal event, match device
      else if((link->device == 0) || (link->device == evt->button.device)) { 
	act.action.device = evt->button.device;
	act.action.actionid = link->action;
	act.action.state = (evt->type == OI_MOUSEBUTTONDOWN);
	action_statepost(&act);
	debug("action_process: %u (mouse button)", act.action.actionid);
      }

      // Next item
      link = link->next;
    }
  }

  // Real: Mouse move
  if(evt->type == OI_MOUSEMOVE) {

    link = action_mouse[OIP_MOTION];

    // Parse actions
    while(link != NULL) {
      // Match device
      if((link->device == 0) || (link->device == evt->move.device)) {
	act.action.device = evt->move.device;
	act.action.actionid = link->action;
	act.action.state = TRUE;
	act.action.data1 = evt->move.relx;
	act.action.data2 = evt->move.rely;
	action_statepost(&act);
	debug("action_process: %u (mouse motion)", act.action.actionid);
      }

      // Next item
      link = link->next;
    }
  }
  else {
    // Zero state table for mouse movement
    link = action_mouse[OIP_MOTION];
    while(link != NULL) {
      if((link->device == 0) || (link->device == evt->move.device)) {
	action_state[link->action] = FALSE;	
      }
      link = link->next;
    }    
  }
}

/* ******************************************************************** */

/**
 * @ingroup IAction
 * @brief Clear lookup link table
 *
 * @param tab pointer to table (array of linked-list heads)
 * @param num number of elements in list
 *
 * Free an entire mouse/keyboard/whatever list-link table
 * by parsing every list entry: Each list entry is the head
 * in a linked list, in which all elements are freed
 */
void action_cleartable(oi_aclink *tab[], uint num) {
  oi_aclink *prev;
  oi_aclink *next;
  uint i;

  // Parse array of linked lists
  for(i=0; i<num; i++) {
    prev = tab[i];
    while(prev != NULL) {
      next = prev->next;
      free(prev);
      prev = next;
    }
  }
}

/* ******************************************************************** */

/**
 * @ingroup IAction
 * @brief Find last element in linked list
 *
 * @param head pointer to head of linked list
 * @param alloc true (1) if returned item should be newly allocated tail,
 *   false (0) otherwise
 * @returns tail of linked list or newly allocated tail (see alloc parameter)
 *
 * Scan linked list, and return tail element
 */
oi_aclink *action_tail(oi_aclink **head, uint alloc) {
  oi_aclink *last;

  // Find tail
  last = *head;
  if(last != NULL) {
    while(last->next != NULL) {
      last = last->next;
    }
  }

  // Should we alloc a new one?
  if(alloc) {
    // Tail was also head
    if(last == NULL) {
      last = (oi_aclink*)malloc(sizeof(oi_aclink));
      *head = last;
      debug("action_tail: tail created");
    }

    // Tail was an element, increment
    else {
      last->next = (oi_aclink*)malloc(sizeof(oi_aclink));
      last = last->next;
      debug("action_tail: tail appended");
    }

    // Be nice and clear members
    last->action = 0;
    last->device = 0;
    last->next = NULL;
  }

  return last;
}

/* ******************************************************************** */

/**
 * @ingroup IAction
 * @brief Update action state table and post event
 *
 * @param evt pointer to event
 *
 * Since a single event can create multiple actions, the
 * state table may need multiple updates for each event.
 * Also, each action event must be posted into the queue.
 */
inline void action_statepost(oi_event *evt) {
  // Dummy check
  if(evt->type != OI_ACTION) {
    return;
  }

  // Update state table
  action_state[evt->action.actionid] = evt->action.state;

  // Post the action event
  queue_add(evt);
}

/* ******************************************************************** */
