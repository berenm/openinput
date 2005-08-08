/*
 * mouse.c : Mouse button and movement state interface
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
#include <string.h>
#include "openinput.h"
#include "internal.h"

// Globals
static oi_device *mousedev;
static sint crd_x;
static sint crd_y;
static sint rel_x;
static sint rel_y;
static sint button;

/* ******************************************************************** */

/**
 * @ingroup IMouse
 * @brief Initialize mouse state manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. The function prepares
 * the mouse manager for use.
 */
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
    if((mousedev->provides & OI_PRO_MOUSE) == OI_PRO_MOUSE) {
      break;
    }
    i++;
  }

  // We really want a device
  if(mousedev == NULL) {
    return OI_ERR_NO_DEVICE;
  }

  debug("mouse_init: mouse device is '%s'", mousedev->name);

  // Done
  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IMouse
 * @brief Mouse motion update
 *
 * @param x horizontal position/movement
 * @param y vertical position/movement
 * @param relative false (0) if x,y is absolute coordinate, true
 *   if x,y is relative movement
 * @param postdev device index of sender, 0 disables posting
 *
 * Inject absolute and relative mouse movement into the mouse
 * state manager. The x,y parameters should both be in pixels.
 *
 * Device drivers which resembles pointer devices probably wants
 * to use this function to inject events into the library.
 */
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
    oi_event ev;
    ev.type = OI_MOUSEMOVE;
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

/**
 * @ingroup IMouse
 * @brief Mouse button update
 *
 * @param btn button index
 * @param state pressed (true) or released (false)
 * @param postdev device index of sender, 0 disables posting
 *
 * Feed mouse button press/release into mouse state manager.
 */
void mouse_button(oi_mouse btn, sint state, uchar postdev) {
  oi_mouse newbutton;
  uchar type;

  newbutton = button;

  if(state) {
    // Down
    type = OI_MOUSEBUTTONDOWN;
    newbutton |= OI_BUTTON_MASK(btn);
  }
  else {
    // Up
    type = OI_MOUSEBUTTONUP;
    newbutton = newbutton - (newbutton & OI_BUTTON_MASK(btn));
  }

  // If nothing changed, bail out
  if(newbutton == button) {
    return;
  }

  // Store state
  button = newbutton;
  
  // Postal services
  if(postdev) {
    oi_event ev;
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

/**
 * @ingroup PMouse
 * @brief Get absolute position of mouse pointer
 *
 * @param x pointer to horizontal position
 * @param y pointer to vertical position
 * @returns mouse button mask, see @ref PMouseMask
 *
 * Get the absolute position of the mouse cursor
 * (stored in the integer pointers) and the current
 * button state as a button mask.
 */
sint oi_mouse_absolute(sint *x, sint *y) {
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

/**
 * @ingroup PMouse
 * @brief Get relative mouse motion
 *
 * @param x pointer to horizontal motion
 * @param y pointer to vertical motion
 * @returns mouse button mask, see @ref PMouseMask
 *
 * Get the relative mouse motion since last call
 * to this function (ie. it is cummulative).
 */
sint oi_mouse_relative(sint *x, sint *y) {
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

/**
 * @ingroup PMouse
 * @brief Warp (move) mouse pointer
 *
 * @param x absolute horizontal position of cursor
 * @param y absolute vertical position of cursor
 * @returns errorcode, see @ref PErrors
 *
 * Move the mouse cursor to the defined absolute
 * position. The function generates a mouse
 * movement event.
 */
sint oi_mouse_warp(sint x, sint y) {
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
  if((oi_app_cursor(OI_QUERY) == OI_DISABLE) &&
     (oi_app_grab(OI_QUERY) == OI_ENABLE)) {
    mouse_move(x, y, FALSE, mousedev->index);  
    e = OI_ERR_OK;
  }

  // Warp default mouse device - driver must generate motion event!
  else if(mousedev->warp) {
    e = mousedev->warp(mousedev, x, y);
  }

  // No default device to perform the warp!
  else {
    e = OI_ERR_NO_DEVICE;
  }

  return e;
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Return string from event-code
 *
 * @param button Button or mouse motion code
 * @returns string containing button or motion name
 *
 * Get the "symbolic" name for a mouse event like
 * buttons and motion.
 */
char *oi_mouse_getname(oi_mouse button) {
  // Dead simple
  switch(button) {
  case OIP_BUTTON_LEFT:
    return "mouse_button_left";

  case OIP_BUTTON_RIGHT:
    return "mouse_button_right";

  case OIP_BUTTON_MIDDLE:
    return "mouse_button_middle";

  case OIP_WHEEL_UP:
    return "mouse_wheel_up";

  case OIP_WHEEL_DOWN:
    return "mouse_wheel_down";

  case OIP_MOTION:
    return "mouse_motion";

  default:
    return "mouse_unknown";
  }
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Get mouse button/motion code given string
 *
 * @param name string name of button/motion
 * @returns mouse button/motion code
 *
 * Translate symbolic string into mouse code.
 */
oi_mouse oi_mouse_getcode(char *name) {
  // Dummies
  if(!name) {
    return OIP_UNKNOWN;
  }
  if((strlen(name) < OI_MIN_KEYLENGTH) ||
     (strlen(name) > OI_MAX_KEYLENGTH)) {
    return OIP_UNKNOWN;
  }

  // Check prefix
  if(strncmp(name, "mouse_", 6) != 0) {
    return OIP_UNKNOWN;
  }

  // Do the comparisons
  if(strcmp(name, "mouse_button_left") == 0) {
    return OIP_BUTTON_LEFT;
  }
  if(strcmp(name, "mouse_button_right") == 0) {
    return OIP_BUTTON_RIGHT;
  }
  if(strcmp(name, "mouse_button_middle") == 0) {
    return OIP_BUTTON_MIDDLE;
  }
  if(strcmp(name, "mouse_wheel_up") == 0) {
    return OIP_WHEEL_UP;
  }
  if(strcmp(name, "mouse_wheel_down") == 0) {
    return OIP_WHEEL_DOWN;
  }
  if(strcmp(name, "mouse_motion") == 0) {
    return OIP_MOTION;
  }

  // We shouldn't reach this point
  return OIP_UNKNOWN;
}

/* ******************************************************************** */
