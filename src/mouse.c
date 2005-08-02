/*
 * mouse.c : Mouse button and movement state interface
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
static sinp_device *mousedev;
static sint crd_x;
static sint crd_y;
static sint rel_x;
static sint rel_y;
static sint button;

/* ******************************************************************** */

// Initialize mouse states (internal)
sint mouse_init() {
  int i;

  // Set globals
  crd_x = 0;
  crd_y = 0;
  rel_x = 0;
  rel_y = 0;
  button = 0;

  // Find default/first mouse device
  i = 1;
  while((mousedev = device_get(i)) != NULL) {
    if((mousedev->provides & SINP_PRO_MOUSE) == SINP_PRO_MOUSE) {
      break;
    }
    i++;
  }

  // We really want a device
  if(mousedev == NULL) {
    return SINP_ERR_NO_DEVICE;
  }

  debug("mouse_init: mouse device is '%s'", mousedev->name);

  // Done
  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Update mouse motion (internal)
void mouse_move(sint x, sint y, sint relative, uchar postdev) {
  sint nx;
  sint ny;
  sint rx;
  sint ry;

  // Always make x,y absolute
  rx = x;
  ry = y;
  if(relative) {
    x = crd_x + x;
    y = crd_y + y;
  }

  // Check absolute position of mouse wrt. window size
  if(x < 0) {
    nx = 0;
  }
  else if(x >= appstate_width()) {
    nx = appstate_width()-1;
  }
  else {
    nx = x;
  }
  
  if(y < 0) {
    ny = 0;
  }
  else if(y >= appstate_height()) {
    ny = appstate_height()-1;
  }
  else {
    ny = y;
  }

  // Calculate relative motion, but only if inside window
  if(!relative && (crd_x >= 0) && (crd_y >= 0)) {
    rx = nx - crd_x;
    ry = ny - crd_y;
  }
  
  // Ok, everything calculated - set global state
  crd_x = nx;
  crd_y = ny;
  rel_x += rx;
  rel_y += ry;

  //FIXME some platforms may need manual pushing of cursor
  
  // Postal services
  if(postdev) {
    sinp_event ev;
    ev.type = SINP_MOUSEMOVE;
    ev.move.device = postdev;
    ev.move.state = button;
    ev.move.x = crd_x;
    ev.move.y = crd_y;
    // Only send relative, not cummulative, ticks (thanks discipline!)
    ev.move.relx = rx;
    ev.move.rely = ry;

    queue_add(&ev);
  }
}

/* ******************************************************************** */

// Update mouse button
void mouse_button(sint btn, sint state, uchar postdev) {
  sint newbutton;
  uchar type;

  newbutton = button;

  if(state) {
    // Down
    type = SINP_MOUSEBUTTONDOWN;
    newbutton |= SINP_BUTTON_MASK(btn);
  }
  else {
    // Up
    type = SINP_MOUSEBUTTONUP;
    newbutton = newbutton - (newbutton & SINP_BUTTON_MASK(btn));
  }

  // If nothing changed, bail out
  if(newbutton == button) {
    return;
  }

  // Store state
  button = newbutton;
  
  // Postal services
  if(postdev) {
    sinp_event ev;
    ev.type = type;
    ev.button.device = postdev;
    ev.button.button = btn;
    ev.button.state = button;
    ev.button.x = crd_x;
    ev.button.y = crd_y;
    queue_add(&ev);
  }
}

/* ******************************************************************** */

// Get absolute position of mouse (public)
sint sinp_mouse_absolute(sint *x, sint *y) {
  // Check current position
  if(crd_x < 0) {
    crd_x = 0;
  }
  if(crd_y < 0) {
    crd_y = 0;
  }  

  // Set data
  if(x) {
    *x = crd_x;
  }
  if(y) {
    *y = crd_y;
  }

  // Return button state
  return button;
}

/* ******************************************************************** */

// Get relative position of mouse (public)
sint sinp_mouse_relative(sint *x, sint *y) {
  // Set data
  if(x) {
    *x = rel_x;
  }
  if(y) {
    *y = rel_y;
  }

  // Reset deltas
  rel_x = 0;
  rel_y = 0;

  // Return button state
  return button;
}

/* ******************************************************************** */

// Warp (set position of) mouse (public)
sint sinp_mouse_warp(sint x, sint y) {
  int e;

  // Dummy checking
  if(x < 0) {
    x = 0;
  }
  else if(x > appstate_width()) {
    x = appstate_width() -1;
  }

  if(y < 0) {
    y = 0;
  }
  else if(y > appstate_height()) {
    y = appstate_height() - 1;
  }

  // If mouse is hidden and grabbed, don't physically move it
  if((sinp_app_cursor(SINP_QUERY) == SINP_DISABLE) &&
     (sinp_app_grab(SINP_QUERY) == SINP_ENABLE)) {
    mouse_move(x, y, FALSE, mousedev->index);  
    e = SINP_ERR_OK;
  }

  // Warp default mouse device - driver must generate motion event!
  else if(mousedev->warp) {
    e = mousedev->warp(mousedev, x, y);
  }

  // No default device to perform the warp!
  else {
    e = SINP_ERR_NO_DEVICE;
  }

  return e;
}

/* ******************************************************************** */

// Return mouse button name (public)
char *sinp_mouse_getname(sinp_mouse button) {
  // Dead simple
  switch(button) {
  case SP_BUTTON_LEFT:
    return "mouse_button_left";

  case SP_BUTTON_RIGHT:
    return "mouse_button_right";

  case SP_BUTTON_MIDDLE:
    return "mouse_button_middle";

  case SP_WHEEL_UP:
    return "mouse_wheel_up";

  case SP_WHEEL_DOWN:
    return "mouse_wheel_down";

  case SP_MOTION:
    return "mouse_motion";

  default:
    return "mouse_unknown";
  }
}

/* ******************************************************************** */

// Return mouse-id for name (public)
sinp_mouse sinp_mouse_getcode(char *name) {
  // Dummies
  if(!name) {
    return SP_UNKNOWN;
  }
  if((strlen(name) < SINP_MIN_KEYLENGTH) ||
     (strlen(name) > SINP_MAX_KEYLENGTH)) {
    return SP_UNKNOWN;
  }

  // Check prefix
  if(strncmp(name, "mouse_", 6) != 0) {
    return SP_UNKNOWN;
  }

  // Do the comparisons
  if(strcmp(name, "mouse_button_left") == 0) {
    return SP_BUTTON_LEFT;
  }
  if(strcmp(name, "mouse_button_right") == 0) {
    return SP_BUTTON_RIGHT;
  }
  if(strcmp(name, "mouse_button_middle") == 0) {
    return SP_BUTTON_MIDDLE;
  }
  if(strcmp(name, "mouse_wheel_up") == 0) {
    return SP_WHEEL_UP;
  }
  if(strcmp(name, "mouse_wheel_down") == 0) {
    return SP_WHEEL_DOWN;
  }
  if(strcmp(name, "mouse_motion") == 0) {
    return SP_MOTION;
  }

  // We shouldn't reach this point
  return SP_UNKNOWN;
}

/* ******************************************************************** */
