/*
 * appstate.c : Application state (focus/grab/cursor) interface 
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
static sinp_device *windowdev;
static sint focus;
static sinp_bool grab;
static sinp_bool cursor;
static sint win_width;
static sint win_height;

/* ******************************************************************** */

// Initialize application state (internal)
sint appstate_init() {
  int i;

  // Ok, our focus is complete
  focus = SINP_FOCUS_MOUSE | SINP_FOCUS_INPUT | SINP_FOCUS_VISIBLE;

  // Not grabbed, and cursor is visible
  grab = FALSE;
  cursor = TRUE;

  // Find default/first window device
  i = 1;
  while((windowdev = device_get(i)) != NULL) {
    if((windowdev->provides & SINP_PRO_WINDOW) == SINP_PRO_WINDOW) {
      break;
    }
    i++;
  }

  // We really want a window
  if(windowdev == NULL) {
    return SINP_ERR_NO_DEVICE;
  }

  // Get window metrics
  if(windowdev->winsize) {
    windowdev->winsize(windowdev, &win_width, &win_height);
  }
  else {
    // No winsize function, very bad
    return SINP_ERR_NOT_IMPLEM;
  }

  debug("appstate_init: window device is '%s'", windowdev->name);

  // Done
  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Set focus state (internal)
void appstate_focus(sint gain, sint state, uchar postdev) {
  sint newfocus;

  // Loose or gain
  newfocus = focus;
  if(gain) {
    newfocus |= state;
  }
  else {
    newfocus = focus - (focus & state);
  }

  // If nothing changed, bail out
  if(newfocus == focus) {
    return;
  }

  // Store state
  focus = newfocus;

  // Postal services
  if(postdev) {
    sinp_event ev;
    ev.type = SINP_ACTIVE;
    ev.active.device = postdev;
    ev.active.gain = gain & TRUE;
    ev.active.state = state;
    queue_add(&ev);
  }
}

/* ******************************************************************** */

// Update window size changes
void appstate_resize(sint w, sint h, uchar postdev) {
  // Store state
  win_width = w;
  win_height = h;

  // Postal services
  if(postdev) {
    sinp_event ev;
    ev.type = SINP_RESIZE;
    ev.resize.device = postdev;
    ev.resize.width = w;
    ev.resize.height = h;
    queue_add(&ev);
  }
}

/* ******************************************************************** */
   
// Return window width (internal)
inline sint appstate_width() {
  return win_width;
}

/* ******************************************************************** */

// Return window height (internal)
inline sint appstate_height() {
  return win_height;
}

/* ******************************************************************** */

// Get focus state of application (public)
sint sinp_app_focus() {
  return focus;
}

/* ******************************************************************** */

// Show/hide cursor (public)
sinp_bool sinp_app_cursor(sinp_bool q) {
  int hide;

  switch(q) {
  case SINP_ENABLE:
    hide = FALSE;
    break;
    
  case SINP_DISABLE:
    hide = TRUE;
    break;

  case SINP_QUERY:
    if(cursor) {
      return SINP_ENABLE;
    }
    else {
      return SINP_DISABLE;
    }
  }
  
  // Set cursor mode on all devices
  {
    sinp_device *dev;
    int i = 1;
    while((dev = device_get(i)) != NULL) {
      // Hide is an optional function
      if(dev->hide) {
	dev->hide(dev, hide);
      }
      i++;
    }
  }
  
  // Remember mode
  cursor = hide;

  return SINP_QUERY;
}

/* ******************************************************************** */

// Grab/ungrab input (public)
sint sinp_app_grab(sinp_bool q) {
  int eat;
  
  switch(q) {
  case SINP_ENABLE:
    eat = TRUE;
    break;
    
  case SINP_DISABLE:
    eat = FALSE;
    break;

  case SINP_QUERY:
    if(grab) {
      return SINP_ENABLE;
    }
    else {
      return SINP_DISABLE;
    }
  }
  
  // Set cursor mode on all devices
  {
    sinp_device *dev;
    int i = 1;
    while((dev = device_get(i)) != NULL) {
      // Grab is an optional function
      if(dev->grab) {      
	dev->grab(dev, eat);
      }
      i++;
    }
  }
  
  // Remember mode
  grab = eat;

  return SINP_QUERY;
}

/* ******************************************************************** */
