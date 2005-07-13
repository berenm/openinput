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
#include <unistd.h>
#include <X11/Xlib.h>
#include "internal.h"
#include "bootstrap.h"
#include "x11.h"

// Bootstrap global
sinp_bootstrap x11_bootstrap = {
  "x11",
  "X11 Window system",
  SINP_PRO_KEYBOARD | SINP_PRO_MOUSE | SINP_PRO_WINDOW,
  x11_avail,
  x11_device,
};

// Private structure
typedef struct x11_private {
  Display *disp;
  Window win;
  Screen *screen;
  Cursor cursor;
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

  // Clear structures
  memset(dev, 0, sizeof(sinp_device));
  memset(priv, 0, sizeof(x11_private));

  // Set members
  dev->private = priv;
  dev->init = x11_init;
  dev->destroy = x11_destroy;
  dev->process = x11_process;  
  dev->grab = x11_grab;
  dev->warp = x11_warp;
  dev->winsize = x11_winsize;
  
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

  // Setup remaining private members
  priv->cursor = x11_mkcursor(priv->disp, priv->win);

  // Install error handlers
  XSetErrorHandler(x11_error);
  XSetIOErrorHandler(x11_fatal);

  // Start receiving events
  XSelectInput(priv->disp, priv->win, FocusChangeMask | KeyPressMask |
	       KeyReleaseMask | PropertyChangeMask | StructureNotifyMask |
	       KeymapStateMask | ButtonPressMask | ButtonReleaseMask |
	       PointerMotionMask);

  //FIXME flush and send!

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Clear X11 'device' block
sint x11_destroy(sinp_device *dev) {
  x11_private *priv;

  debug("x11_destroy");

  if(dev) {
    priv = (x11_private*)dev->private;

    // Private members
    if(priv) {
      if(priv->cursor) {
	XFreeCursor(priv->disp, priv->cursor);
      }
      free(priv);
    }

    // Device struct
    free(dev);
    dev = NULL;
  }
  
  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Pump event into the queue
void x11_process(sinp_device *dev) {
  x11_private *priv;

  priv = (x11_private*)dev->private;
  debug("x11_process");

  // Process all pending events
  while(sinp_runstate() && x11_pending(priv->disp)) {
    x11_dispatch(priv->disp);
  }
}

/* ******************************************************************** */

// Grab (lock) mouse/keyboard in window
sint x11_grab(sinp_device *dev, sint on) {
  x11_private *priv;
  int i;

  priv = (x11_private*)dev->private;
  debug("x11_grab");

  if(on) {
    XGrabKeyboard(priv->disp, priv->win, TRUE,
		  GrabModeAsync, GrabModeAsync, CurrentTime);
    
    // Wait for succesfull grabbing of mouse 
    while(1) {
      i = XGrabPointer(priv->disp, priv->win, TRUE,
		       ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
		       GrabModeAsync, GrabModeAsync,
		       priv->win, None, CurrentTime);
      if(i == GrabSuccess) {
	break;
      }
      usleep(SINP_SLEEP);
    }
  }
  else {
    // Simply ungrab both
    XUngrabKeyboard(priv->disp, CurrentTime);
    XUngrabPointer(priv->disp, CurrentTime);
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Show/hide mouse cursor
sint x11_hidecursor(sinp_device *dev, sint on) {
  x11_private *priv;

  debug("x11_hidecursor");
  priv = (x11_private*)dev->private;

  // Hide - set blank cursor
  if(on) {
    XDefineCursor(priv->disp, priv->win, priv->cursor);
  }

  // Show - set default cursor
  else {
    XDefineCursor(priv->disp, priv->win, None);
  }

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Warp mouse
sint x11_warp(sinp_device *dev, sint x, sint y) {
  x11_private *priv;

  priv = (x11_private*)dev->private;

  // Simply go to warp speed
  XWarpPointer(priv->disp, None, priv->win, 0, 0, 0, 0, x, y);
  XSync(priv->disp, False);

  return SINP_ERR_OK;
}

/* ******************************************************************** */

// Get window sizes
sint x11_winsize(sinp_device *dev, sint *w, sint *h) {
  x11_private *priv;
  XWindowAttributes attr;

  debug("x11_windsize");
  priv = (x11_private*)dev->private;
  
  XGetWindowAttributes(priv->disp, priv->win, &attr);

  *w = attr.width;
  *h = attr.height;
  
  return SINP_ERR_OK;
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

// Make 'empty' mouse cursor
Cursor x11_mkcursor(Display *d, Window w) {
  char bitmap[] = {0};
  Pixmap pixmap;
  XColor color;
  Cursor cursor;
  
  pixmap = XCreateBitmapFromData(d, w, bitmap, 1, 1);
  cursor = XCreatePixmapCursor(d, pixmap, pixmap,
			       &color, &color, 0, 0);
  XFreePixmap(d, pixmap);

  return cursor;
}

/* ******************************************************************** */
