/*
 * x11.c : X11 utility functions (bootstrapping, etc.)
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
#include "internal.h"
#include "bootstrap.h"
#include "x11.h"

// Bootstrap global
sinp_bootstrap x11_bootstrap = {
  "x11",
  "X11 Window system",
  SINP_PRO_KEYBOARD | SINP_PRO_POINTER | SINP_PRO_HIDECUR,
  x11_avail,
  x11_device,
};

// Private structure
typedef struct x11_private {
  Display *disp;
  Window win;
  Screen *screen;
  Cursor cursor;
  uint grabmask;
  sint winheight;
  sint winwidth;
  sint mousex;
  sint mousey;
  sint oldx;
  sint oldy;
} x11_private;

/* ******************************************************************** */
 
// Bootstrap: Check availablity of X11
sint x11_avail() {
  Display *disp;

  debug("x11_avail");

  // Check for X11 existence
  disp = XOpenDisplay(NULL);
  if(disp != NULL) {
    // Got it, close display again
    XCloseDisplay(disp);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/* ******************************************************************** */

// Bootstrap: Install X11 'device' block
sinp_device *x11_device() {
  sinp_device *dev;
  x11_private *priv;

  debug("x11_device");

  // Alloc
  dev = (sinp_device*)malloc(sizeof(sinp_device));
  priv = (x11_private*)malloc(sizeof(x11_private));
  if((dev == NULL) || (priv == NULL)) {
    debug("x11_device: device creation failed");
    if(dev) {
      free(dev);
    }
    if(priv) {
      free(priv);
    }
    return NULL;
  }

  // Clear private
  memset(priv, 0, sizeof(x11_private));

  // Set members
  memset(dev, 0, sizeof(sinp_device));
  dev->init = x11_init;
  dev->enable = x11_enable;
  dev->destroy = x11_destroy;
  dev->process = x11_process;  
  dev->grab = x11_grab;
  dev->private = priv;
  
  // Done
  return dev;
}

/* ******************************************************************** */

// Initialize X11
sint x11_init(sinp_device *dev, char *window_id, uint flags) {
  x11_private *priv;

  priv = (x11_private*)dev->private;
  debug("x11_init");

  // Parse the window_id flags
  priv->disp = (Display*)device_windowid(window_id, SINP_I_CONN);
  priv->screen = (Screen*)device_windowid(window_id, SINP_I_SCRN);
  priv->win = (Window)device_windowid(window_id, SINP_I_WINID);

  // We require conn and winid parameters
  if(!(priv->disp) || !(priv->win)) {
    debug("x11_init: conn (c) and winid (w) parameters required\n");
    return SINP_ERR_NO_DEVICE;
  }

  // Setup a null-cursor here

  // Install error handlers
  XSetErrorHandler(x11_error);
  XSetIOErrorHandler(x11_fatal);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Enable X11 driver
sint x11_enable(sinp_device *dev, sint on) {
  long mask;
  x11_private *priv;

  debug("x11_enable");

  // Turn on fetching
  if(on) { 
    // Select all usefull inputs
    mask = FocusChangeMask | KeyPressMask | KeyReleaseMask |
      PropertyChangeMask | StructureNotifyMask | KeymapStateMask |
      ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
  }
  else {
    // Select no inputs
    mask = NoEventMask;
  }

  debug("x11_enable: set mask %u", mask);
  
  priv = (x11_private*)dev->private;
  XSelectInput(priv->disp, priv->win, mask);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Clear X11 'device' block
sint x11_destroy(sinp_device *dev) {
  debug("x11_destroy");

  if(dev) {
    if(dev->private) {
      free(dev->private);
    }
    free(dev);
    dev = NULL;
  }
  
  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Pump event into the queue
void x11_process(sinp_device *dev) {
  debug("x11_process");

  //FIXME HERE do processing!!!
}

/* ******************************************************************** */

// Grab (lock) mouse/keyboard in window
sint x11_grab(sinp_device *dev, uint mask) {
  x11_private *priv;
  sint ok;

  priv = (x11_private*)dev->private;
  ok = FALSE;
  debug("x11_grab");

  // Keyboard
  if(mask & SINP_PRO_KEYBOARD) {
    // Grab on
    priv->grabmask |= SINP_PRO_KEYBOARD;
    XGrabKeyboard(priv->disp, priv->win, TRUE,
		  GrabModeAsync, GrabModeAsync, CurrentTime);
    ok = TRUE;
  }
  if(!(mask & SINP_PRO_KEYBOARD)) {
    // Grab off
    priv->grabmask = priv->grabmask - (priv->grabmask & SINP_PRO_KEYBOARD);
    XUngrabKeyboard(priv->disp, CurrentTime);
    ok = TRUE;
  }
  
  // Pointer
  if(mask & SINP_PRO_POINTER) {
    // Grab on
    priv->grabmask |= SINP_PRO_POINTER;
    XGrabPointer(priv->disp, priv->win, TRUE,
		 ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
		 GrabModeAsync, GrabModeAsync,
		 priv->win, None, CurrentTime);
    ok = TRUE;
  }
  if(!(mask & SINP_PRO_POINTER)) {
    // Grab off
    priv->grabmask = priv->grabmask - (priv->grabmask & SINP_PRO_POINTER);
    XUngrabPointer(priv->disp, CurrentTime);
    ok = TRUE;
  }

  // Cursor
  if(mask & SINP_PRO_HIDECUR) {
    // Hide
    priv->grabmask |= SINP_PRO_HIDECUR;
    //FIXME implement
    ok = TRUE;
  }
  if(!(mask & SINP_PRO_HIDECUR) {
    // Show
    priv->grabmask = priv->grabmask - (priv->grabmask & SINP_PRO_HIDECUR);
    //FIXME implement
    ok = TRUE;
  }

  // Report errors
  if(ok) {
    return SINP_ERR_OK;
  }
  else {
    return SINP_ERR_NOT_IMPLEM;
  }
}

/* ******************************************************************** */

// X11 error
sint x11_error(Display *d, XErrorEvent *e) {
  debug("x11_error: code %u", e->error_code);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// X11 fatal I/O error
sint x11_fatal(Display *d) {
  debug("x11_fatal: fatal I/O error");

  return SINP_ERR_OK;
}

/* ******************************************************************** */
