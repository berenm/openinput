/*
 * appstate.c : Application state (focus/grab/cursor) interface 
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
static oi_device *windowdev;
static sint focus;
static oi_bool grab;
static oi_bool cursor;
static sint win_width;
static sint win_height;

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Initialize application state manager
 *
 * Must be called on library initialization.
 */
sint appstate_init() {
  int i;

  // Ok, our focus is complete
  focus = OI_FOCUS_MOUSE | OI_FOCUS_INPUT | OI_FOCUS_VISIBLE;

  // Not grabbed, and cursor is visible
  grab = FALSE;
  cursor = TRUE;

  // Find default/first window device
  i = 1;
  while((windowdev = device_get(i)) != NULL) {
    if((windowdev->provides & OI_PRO_WINDOW) == OI_PRO_WINDOW) {
      break;
    }
    i++;
  }

  // We really want a window
  if(windowdev == NULL) {
    return OI_ERR_NO_DEVICE;
  }

  // Get window metrics
  if(windowdev->winsize) {
    windowdev->winsize(windowdev, &win_width, &win_height);
  }
  else {
    // No winsize function, very bad
    return OI_ERR_NOT_IMPLEM;
  }

  debug("appstate_init: window device is '%s'", windowdev->name);

  // Done
  return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Update focus state
 *
 * @param gain focus gained:1 or lost:0
 * @param state type of focus gained/lost
 * @param postdev device index of sender, 0 disables event posting
 *
 * Update the focus state of the application (ie. window).
 * Rhree types of focus exists: #OI_FOCUS_MOUSE #OI_FOCUS_INPUT and
 * #OI_FOCUS_VISIBLE
 */
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
    oi_event ev;
    ev.type = OI_ACTIVE;
    ev.active.device = postdev;
    ev.active.gain = gain & TRUE;
    ev.active.state = state;
    queue_add(&ev);
  }
}

/* ******************************************************************** */

// Update window size changes
/**
 * @ingroup IAppstate
 * @brief Update application window size state
 *
 * @param w width of window
 * @param h height of window
 * @param postdev device index of sender, 0 disables event posting
 *
 * Update the window size state.
 */
void appstate_resize(sint w, sint h, uchar postdev) {
  // Store state
  win_width = w;
  win_height = h;

  // Postal services
  if(postdev) {
    oi_event ev;
    ev.type = OI_RESIZE;
    ev.resize.device = postdev;
    ev.resize.width = w;
    ev.resize.height = h;
    queue_add(&ev);
  }
}

/* ******************************************************************** */
   
/**
 * @ingroup IAppstate
 * @brief Get application window width
 *
 * @returns window width (pixels)
 */
inline sint appstate_width() {
  return win_width;
}

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Get application window height
 *
 * @returns window height (pixels)
 */
inline sint appstate_height() {
  return win_height;
}

/* ******************************************************************** */

/**
 * @ingroup PAppstate
 * @brief Get application focus state
 *
 * @returns focus bitmask
 *
 * A bitmask comprised of #OI_FOCUS_MOUSE #OI_FOCUS_INPUT or
 * #OI_FOCUS_VISIBLE
 */
sint oi_app_focus() {
  return focus;
}

/* ******************************************************************** */

/**
 * @ingroup PAppstate
 * @brief Hide or display the mouse cursor
 *
 * @param q new cursor state or query
 * @returns state of cursor visiblity
 *
 * Show or hide the mouse cursor. The state can also
 * be queried using #OI_QUERY
 */
oi_bool oi_app_cursor(oi_bool q) {
  int hide;

  switch(q) {
  case OI_ENABLE:
    hide = FALSE;
    break;
    
  case OI_DISABLE:
    hide = TRUE;
    break;

  case OI_QUERY:
    if(cursor) {
      return OI_ENABLE;
    }
    else {
      return OI_DISABLE;
    }
  }
  
  // Set cursor mode on all devices
  {
    oi_device *dev;
    int i = 1;
    while((dev = device_get(i)) != NULL) {
      // Hide is an optional function
      if(dev->hide) {
	dev->hide(dev, hide);
      }

      // When unhiding, warp the mouse due to fudge
      if(!hide && dev->warp) {
	sint x;
	sint y;
	oi_mouse_absolute(&x, &y);
	dev->warp(dev, x, y);
      }
      i++;
    }
  }
  
  // Remember mode
  cursor = hide;

  return OI_QUERY;
}

/* ******************************************************************** */

/**
 * @ingroup PAppstate
 * @brief Grab or release the mouse cursor
 * 
 * @param q enable/disable/query grabbing
 * @returns state of grabbing
 *
 * Grab or release the mouse cursor. The grab state can
 * also be queried using #OI_QUERY
 */
sint oi_app_grab(oi_bool q) {
  int eat;
  
  switch(q) {
  case OI_ENABLE:
    eat = TRUE;
    break;
    
  case OI_DISABLE:
    eat = FALSE;
    break;

  case OI_QUERY:
    if(grab) {
      return OI_ENABLE;
    }
    else {
      return OI_DISABLE;
    }
  }
  
  // Set cursor mode on all devices
  {
    oi_device *dev;
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

  return OI_QUERY;
}

/* ******************************************************************** */
